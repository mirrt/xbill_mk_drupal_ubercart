<?php

/**
 * @file
 * X-bill menu items.
 *
 */

$mk_config = array();

function uc_xbillpay_payform() {    
    
    foreach ($_POST as $Key=>$Value) {
        $$Key = $Value; 
    }

    if ( !isset($order_id)) {
        print t('An error has occurred during payment.  Please contact us to ensure your order has been submitted.');
        exit();
    }
 
    $order_id = (int)$order_id;
    $order = uc_order_load($order_id);
       
    if ($order === FALSE) {
        print t('An error has occurred during payment.  Please contact us to ensure your order has been submitted.');
        exit();
    }
    
    
    if(isset($phone) and (float) $mix > 0){
      
        $form_sended = TRUE;
        $login = variable_get('uc_xbillpay_login', '');
        $project_id = variable_get('uc_xbillpay_id', '');
        $skey  = variable_get('uc_xbillpay_key','');
        
        watchdog('X-bill', 'Receiving a payment confirmation for order #!order_id.', array('!order_id' => check_plain($order_id)));
        
        $order_state = uc_order_status_data($order->order_status, 'state');
  
        if ($order_state != 'in_checkout' && $order_state != 'post_checkout'){
            echo t('Declined. The order is paid, complete or canceled.');
            exit();
        }
        
        if ($order_sum == $order->order_total && $xbill_login == $login){
           
            global $mk_config;
           
            $mk_config['usr'] = $login;
            $mk_config['key'] = $skey; 
            $mk_config['sid'] = $project_id; 
            $mk_config['desc'] = variable_get('uc_xbillpay_desc', '');
            $mk_config['answer'] = variable_get('uc_xbillpay_answer', '');
            
            $params_for_back = array('order_id' => $order_id,
                                     'order_sum' => $order_sum);
            
            $payment_mk = mk_create_pay (
                            $phone, 
                            $mix, 
                            $mk_config['desc'], 
                            $mk_config['answer'],
                            $params_for_back  
                          );
            if($payment_mk['status'] == 0){
                
                if (uc_order_status_data($order->order_status, 'state') == 'in_checkout') {
                    db_update('uc_orders')
                        ->fields(array('order_status' => 'processing'))
                        ->condition('order_id', $order_id)
                        ->execute();
                }
                $form_message = 'На Ваш номер телефона отправлена SMS, после подтверждения платежа, баланс будет пополнен моментально.';
            }
            else{
                $form_message = 'При проведении платежа возникла следующая ошибка: <br>';
                $form_message .= $payment_mk['status'] . '-' . $payment_mk['status_desc'];
            }    
 
        }
     }     
     
    header('Content-Type: text/html;charset=utf-8');

    $hidden_fields = array ( 
                        'order_id'    => check_plain($order_id),
                        'order_sum'   => check_plain($order_sum),
                        'xbill_login' => check_plain($xbill_login),
                        'xbill_project_id'    =>  check_plain($xbill_project_id) 
                    );
    
    $mod_path = drupal_get_path('module', 'uc_xbillpay');
    $file_path = base_path() . $mod_path;
    
    if($mod_path != ''){
        require ($mod_path . '/uc_xbillpay.form.inc');
    }
    else {
        print t('An error has occurred during payment.  Please contact us to ensure your order has been submitted.');
        exit();
    }
    
    $_SESSION['uc_checkout'][$_SESSION['cart_order']]['do_complete'] = TRUE;
    //drupal_goto('cart/checkout/complete');
}

function uc_xbillpay_result() {

    foreach ($_GET as $Key=>$Value) {
        $$Key = $Value; 
    }

    $order_mk = intval($order);
    $order_id = intval($order_id);
    $order = uc_order_load($order_id);

    watchdog('X-bill', 'Receiving a payment confirmation for order #!order_id.', array('!order_id' => check_plain($order_id)));
  
    if ($order === FALSE) {
        print 'error(can not find order)';
        exit();
    }
  
    $order_state = uc_order_status_data($order->order_status, 'state');
    
    if ($order_state != 'in_checkout' && $order_state != 'post_checkout'){
        print 'error(an order can not be process)';
        exit();
    }
  
    
    $login = variable_get('uc_xbillpay_login', '');
    $project_id = variable_get('uc_xbillpay_id', '');
    $skey  = variable_get('uc_xbillpay_key','');
    
    
    if ($order_sum == $order->order_total && $xbill_login == $login){
            
        global $mk_config;

        $mk_config['usr'] = $login;
        $mk_config['key'] = $skey; 
        $mk_config['sid'] = $project_id; 
        
        
        if( ! verifySign($_GET)) {
                print 'error(sign)';
                exit();
        }
        
        if($order_status == 'success') {
            uc_order_save($order);
            watchdog('X-bill', 'Payment approved for order #!order_id.', array('!order_id' => check_plain($order_id)));
            $comment = t('Paid. Transaction  Number #!order.', array('!order' => check_plain($oreder_id)));
            uc_payment_enter($order->order_id, 'xbillpay', $order_sum, 0, NULL, $comment);
            uc_cart_complete_sale($order);
            print 'ok';    
        }
        
        $order_id = check_plain($order_id);
        $amount = check_plain($paytouser);
        $payee = check_plain($order_mk);
        $payer = check_plain($phone);
        $description = check_plain($order_status);
  
	if (db_query("SELECT COUNT(order_id) FROM {uc_payment_xbill} WHERE order_id = :s", array(':s' => $order_id))->fetchField()) {
            
            db_query("UPDATE {uc_payment_xbill} SET description = :s WHERE order_id = :d",
                     array(
                        ':s' => $description . ' update',
                        ':d' => $order_id
                     )
            );
         } else {
            db_insert('uc_payment_xbill')
                ->fields( array(
                    'order_id' => $order_id,
                    'amount' => $amount,
                    'payee' => $payee,
                    'payer' => $payer,
                    'description' => $description
                  ))
                ->execute();
        } 
    }  else {
        print 'error(not fount order)';
        exit();
    }
}
   
   
function mk_create_pay ($phone, $cost, $desc, $answer="", $arr=array()) {
    global $mk_config;
    
    $mk_config['api1'] = "http://api.x-bill.org/"; 
    $mk_config['api2'] = "http://api.x-bill.ru/";
    $phone = preg_replace('/[^0-9]/', '', $phone);
    $cost = (float)str_replace(",", ".", $cost);
    $desc = $desc;
    $answer = $answer;
    $var = "";
    if (isset ($arr)) {
            $keys = array_keys ($arr);
            for($i=0; $i<count($keys); $i++){ $var .= "&{$keys[$i]}=".$arr[$keys[$i]]; }
    }
    $post = "phone={$phone}&cost={$cost}&desc={$desc}&answer={$answer}&sign=".mk_create_sign($phone)."&login={$mk_config['usr']}&sid={$mk_config['sid']}{$var}";

    $result = mk_send_data ($post, $mk_config['api1']."payment.php");
    
    if ($result == 'error') { $result = mk_send_data ($post, $mk_config['api2']."payment.php"); }
    if ($result == 'error') {
            return "0";
    }else{
            $result = mk_parse_result($result);
            return $result;
    }
}

function verifySign($request){
    $sign = $request['sign'];
    global $mk_config;
    
    return $sign == md5($request['order'].
                        $request['phone'].
                        $request['merchant_price'].
                        $mk_config['key']);
}

function mk_create_sign ($phone=""){
    global $mk_config;
    return md5($mk_config['usr'].$mk_config['key'].$mk_config['sid'].$phone);
}

function mk_parse_result ($result){
    $XML = trim($result);
    $returnVal = $XML;
    $emptyTag = '<(.*)/>';
    $fullTag = '<\\1></\\1>';
    $XML = preg_replace ("|$emptyTag|", $fullTag, $XML);
    $matches = array();
    if (preg_match_all('|<(.*)>(.*)</\\1>|Ums', trim($XML), $matches)) {
            if (count($matches[1]) > 0) $returnVal = array();
            foreach ($matches[1] as $index => $outerXML){
                    $attribute = $outerXML;
                    $value = mk_parse_result($matches[2][$index]);
                    if (! isset($returnVal[$attribute])) $returnVal[$attribute] = array();
                    $returnVal[$attribute][] = $value;
            }
    }
    if (is_array($returnVal)) foreach ($returnVal as $key => $value){ if (is_array($value) && count($value) == 1 && key($value) === 0){ $returnVal[$key] = $returnVal[$key][0]; } }
    return $returnVal;	
}

function mk_send_data ($post, $url){
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL,$url);
    curl_setopt($ch, CURLOPT_FAILONERROR, 1);
    #curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1); # Разрешить переадресацию
    curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
    curl_setopt($ch, CURLOPT_TIMEOUT, 4); # Таймаут не менять! 
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 60);
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $post);

    $result = curl_exec($ch);
    
    $status = curl_errno($ch);   

    curl_close($ch);   
    if ($status == 0 && !empty($result)) { return $result; }else{ return "error"; }
}   
   