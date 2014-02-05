<style>
	.nn_vkladka { list-style:none; margin:0px; padding:0px; height:28px; }
	.nn_vkladka li { float:left; padding:6px 12px 6px 12px; color:#333; font-size:13px; background-color:#f8f8f8; cursor:pointer; border-right:1px solid #fff; }
	.nn_forms { background-color:#eee; font-size:12px; padding:12px 12px 6px 12px; }
	.nn_h1 { font-size:18px; color:#006699; }
	.nn_title { background-color:#fff; border-bottom:1px solid #ddd; padding:6px 10px 6px 10px; }
	.nn_operator_list { padding-left:40px; padding-right:40px; list-style:none; }
	.nn_operator_list li{ padding:6px 0px 6px 0px; color:#666; border-bottom:1px solid #ddd; }
	.nn_operator_list span{ float:right; color:#222; }
	.nn_input { background-color:#fff; padding:6px 10px 6px 10px; border:1px solid #c6d4dc; outline:none; }
	.nn_rows { padding:10px 0px 10px 0px; }
	.nn_rows p{ float:right; margin-top:-8px; }
	.nn_pravila { padding:0px 10px 0px 10px; }
	.nn_pravila p { background-repeat:no-repeat; background-position:left center; padding:8px 0px 8px 22px; cursor:pointer;  }
	.nn_marker { background-repeat:no-repeat; background-position:right 6px; background-image:url(pic/down.png); padding-right:14px;  }
	.nn_light { font-size:10px; color:#666; display:none; background-color:#f8f8f8; padding:6px; border:1px solid #ddd; }
	.nn_mini { color:#9cacbe; font-size:10px; }
	.nn_button { background-color:#006699; padding:7px 10px 7px 10px; border:none; outline:none; color:#fff; cursor:pointer; -webkit-border-radius:2px;-moz-border-radius:2px;border-radius:2px; }
	.nn_button:hover { background-color:#0770a4; }
	.nn_oper { margin:0px; padding:0px; list-style:none; height:40px; padding-left:6px; }
	.nn_oper li{ background-repeat:no-repeat; background-position:6px center; padding:8px 12px 8px 22px; background-color:#fff; margin-right:3px; margin-left:3px; float:left; -webkit-border-radius:4px;-moz-border-radius:4px;border-radius:4px; }
	.nn_load { background:url('http://myface.su/templates/Default/images/loading_mini.gif') no-repeat right center; width:42px; height:27px; display:none; }
	#nn_help_text { font-size:10px; padding:8px 0px 10px 4px; color:#611d1d; }
</style>
<script type="text/javascript" src="<?php echo base_path()?>/misc/jquery.js"></script>
<script type="text/javascript">
        
	function set_forms(id){
		$("#nn_mk").css('backgroundColor', 'rgb(248,248,248)');
		$("#nn_sms").css('backgroundColor', 'rgb(248,248,248)');
		if (id != 'nn_mhelp') {
		$("#" + id).css('backgroundColor', 'rgb(238,238,238)');
		}
		$("#nn_mk_form").css('display', 'none');
		$("#nn_sms_form").css('display', 'none');
		$("#nn_mhelp_form").css('display', 'none');
		$("#"  + id + "_form").css('display', 'block');
		
	}
	
	function set_help(id){
		if ($("#" + id + "_text").css('display') == 'none') {
			$("#" + id).css('backgroundImage', 'url(pic/up.png)');
			$("#" + id + "_text").css('display', 'block');
		}else{
			$("#" + id).css('backgroundImage', 'url(pic/down.png)');
			$("#" + id + "_text").css('display', 'none');
		}	
	}
	
	function create_pay(){
		$(".nn_load").css('display', 'block');
		$(".nn_button").css('display', 'none');
		var phone = $("#phone").val();
		var mix = $("#mix").val();
		var users = $("#users").val();
		if (phone.length == '12') {
			if (mix > 4 && mix < 5000) {
                            $('#pay_form').submit();
			}else{
				$(".nn_load").css('display', 'none');
				$(".nn_button").css('display', 'block');
				$("#nn_help_text").html("Укажите сумму от 5 до 5000 рублей.");
			}
		}else{
			$(".nn_load").css('display', 'none');
			$(".nn_button").css('display', 'block');
			$("#nn_help_text").html("Укажите номер телефона в формате +79281234567");
		}
	}
</script>

<div class="miniature_box" align ="center">
        <br>
	<div class="miniature_pos" style="width:500px">
		<div class="payment_title">
			<img src="<?php echo $file_path?>/img/xbill.png"  />
			<div class="fl_l">
				Вы выбрали способ оплаты с помощью мобильного платежа.<br />
				Стоимость вашего заказа: <b><?php echo $order_sum?> RUR</b>
			</div>
			<br>
			<div class="clear"></div>
		</div>
		<div class="clear"></div>
		<ul class='nn_vkladka'>
			<li onclick="set_forms(this.id);" id='nn_mk' style='background-color:#eee; border-right:none;'>Мобильный платеж</li>
			<li onclick="set_forms(this.id);" id='nn_mhelp' style='background-color:#dee4ea;'>?</li>
		</ul>
		<div class='nn_forms' id='nn_mk_form'>
                     <?php if ($form_sended):?>
                        <p><?php echo $form_message?></p>
                    <?php else: ?>
			<table cellpadding='1' cellspacing='1'>
                                <form method="post" id="pay_form" name="form_sended">
				<tr>
					<td class='nn_mini'>Сумма:</td>
					<td class='nn_mini'>Номер телефона:</td>
					<td></td>
				</tr>
				<tr>
					<td><input type="text" id='mix' name ="mix" class="nn_input" maxlength='5' style="width:60px;" value="<?php echo $order_sum?>" /></td>
					<td><input type="text" id='phone' name="phone" class="nn_input" maxlength='12' style="width:120px;" value="+7" /></td>
                                        <?php foreach($hidden_fields as $name => $value):?>
                                            <input type="hidden" name="<?php echo $name?>" value="<?php echo $value?>">
                                        <?php endforeach?>
                                        
                                        <td><input class='nn_button' type='button' onclick="create_pay();" value='Оплатить'><p class='nn_load'></p></td>
				</tr>
                                </form>
			</table>
			<div id='nn_help_text'>
			</div>
			<p class='nn_title'>
				Услуга доступна для операторов:
			</p>
			<br />
			<ul class='nn_oper'>
				<li style='background-image:url(<?php echo $file_path?>/img/mf.png);'>Мегафон</li>
				<li style='background-image:url(<?php echo $file_path?>/img/mts.png);'>МТС</li>
				<li style='background-image:url(<?php echo $file_path?>/img/beeline.png);'>Билайн</li>
				<li style='background-image:url(<?php echo $file_path?>/img/smarts.png);'>Смартс</li>
				<li style='background-image:url(<?php echo $file_path?>/img/usi.png);'>Ютел</li>
				<li style='background-image:url(<?php echo $file_path?>/img/tele2.png);'>Теле2</li>
			</ul>
                        <?php endif?>
		</div>
		<div class='nn_forms' id='nn_mhelp_form' style='background-color:#dee4ea; display:none;'>
			<p class='nn_title'>
				Для оплаты заказа через мобильный платеж необходимо выполнить требования оператора:
			</p>
			<br />
						<div class='nn_pravila'>
<p onclick="set_help('marker_mf');" style='background-image:url(<?php echo $file_path?>/img/mf.png);'><span id='marker_mf' class='nn_marker'>Мегафон</span></p>
<span class='nn_light' id='marker_mf_text'>
После списания суммы покупки на вашем счете должно остаться не менее 30 руб.<br />
Минимальная сумма единовременного платежа 1 руб.<br />
Максимальный разовый платеж - 5000 руб.<br />
Максимальная сумма платежей за сутки - 15000 руб.<br />
Максимальная сумма платежей за месяц – 15000 руб.<br />
Максимальная количество платежей за сутки - 10<br />
Максимальная количество платежей за неделю - 20<br />
Максимальная количество платежей за месяц - 50<br />
<br />
«Мобильные платежи» доступны всем абонентам «МегаФона», за исключением юридических лиц и абонентов, обслуживающихся по кредитной системе расчетов.
</span>
<p onclick="set_help('marker_beeline');"  style='background-image:url(<?php echo $file_path?>/img/beeline.png);'><span id='marker_beeline' class='nn_marker'>Билайн</span></p>
<span class='nn_light' id='marker_beeline_text'>
Услуга доступна для абонентов тарифных планов любой системы расчетов, кроме линейки тарифов «Простая логика», «Правильный», а также абонентов, у которых подключены услуги «Безлимит внутри сети» или «Безумные дни».<br />
Оплата возможна только со специального авансового счета.<br />
После списания суммы покупки на Вашем счете должно остаться не менее 50 руб.<br />
Услуга становится доступной с момента расходования вами 150 руб. за услуги связи с момента подключения к сети Билайн.<br />
Минимальная сумма платежа 10 руб.<br />
Максимальный разовый - 15000 руб.<br />
Максимальная сумма платежей за сутки - 30000 руб. максимум 10 транзакций<br />
Максимальная сумма платежей за месяц -30000 руб.<br />
<br />
Если вы пользуетесь тарифом с постоплатной системой расчетов то:<br />
<br />
Услуга становится доступной с момента расходования вами 150 руб. за услуги связи с момента подключения к сети «Билайн».<br />
</span>

<p onclick="set_help('marker_mts');"  style='background-image:url(<?php echo $file_path?>/img/mts.png);'><span id='marker_mts' class='nn_marker'>МТС</span></p>
<span class='nn_light' id='marker_mts_text'>
Стоимость подтверждения покупки (входящая смс с номера 7663) составляет 10 рублей с НДС и взимается помимо комиссии за успешную транзакцию, вне зависимости от суммы платежа.<br />
Минимальная разовая сумма платежа: 1 руб*.<br />
Максимальная разовая сумма платежа: 14 999 руб*.<br />
При оплате услуг МТС максимальная разовая сумма платежа: 3 000 руб., Билайн, Yota и Мегафон – 1 000 руб.<br />
Количество платежей в сутки не более 5 платежей<br />
Максимальная сумма платежей в сутки: 30 000 руб.<br />
Максимальная сумма платежей в месяц: 40 000 руб.<br />
Обязательный остаток на счете: 10 руб*.<br />
<br />
* если нет других указаний от поставщика услуг. <br />
** остаток собственных средств абонента, внесенных им на счет телефона. <br />
<br />
Ваш контракт с МТС должен быть оформлен на физическое лицо (на тарифных планах «Супер Ноль», действующем с 11.05.2011г., «Супер МТС_2011», «Супер МТС 2012» и «Супер МТС» данная услуга не предоставляется)<br />
Действует ограничение по незавершенным операциям (вы не можете сделать новый запрос, если предыдущий необработан)<br />
На вашем номере обслуживания должны отсутствовать опции «Запрет возврата части аванса» и/или «Запрет передачи данных третьим лицам информации об абоненте»<br />
Невозможно использовать кредитные и бонусные средства, а также средства, начисленные по рекламным акциям, скидки, на услуги связи, предоставленные МТС, скидку на первоначальный объем услуг, предоставляемую при заключении договора (покупке комплекта) и т.п.<br />
</span>

<p onclick="set_help('marker_tele2');"  style='background-image:url(<?php echo $file_path?>/img/tele2.png);'><span id='marker_tele2' class='nn_marker'>Теле2</span></p>
<span class='nn_light' id='marker_tele2_text'>
После списания суммы покупки на вашем счете должно остаться не менее 20 руб.<br />
Cервис доступен только физическим лицам, платежи с корпоративных тарифов не разрешены.<br />
Невозможно использовать кредитные и бонусные средства, а также средства, начисленные по рекламным акциям, скидки, на услуги связи, предоставленные Tele2, скидку на первоначальный объем услуг, предоставляемую при заключении договора (покупке комплекта) и т.п.<br />
Минимальная сумма платежа: 10 руб.<br />
Неснижаемый остаток на лицевом счете после платежа	10 (20 - для абонентов Санкт-Петербурга и Ленинградской области)<br />
Максимальная сумма платежа 1 000 руб. - для услуг мобильной связи и Yota, 5 000 руб. - для остальных услуг и категорий<br />
Максимальное число платежей в сутки/месяц: 10/50<br />
Максимальная сумма платежей в сутки/месяц: 5 000/40 000<br />
</span>

<p onclick="set_help('marker_smarts');"  style='background-image:url(<?php echo $file_path?>/img/smarts.png);'><span id='marker_smarts' class='nn_marker'>Смартс</span></p>
<span class='nn_light' id='marker_smarts_text'>
Услуга доступна для абонентов Пенза-GSM авансовой системы расчетов.<br />
<br />
Для того чтобы воспользоваться услугой «Мобильный платеж» наберите *103#1# со своего мобильного телефона. Услуга «Мобильный платеж» доступна физическим лицам — абонентам Пенза-GSM. Подключение услуги — бесплатно.<br />
</span>

<p onclick="set_help('marker_usi');"  style='background-image:url(<?php echo $file_path?>/img/usi.png);'><span id='marker_usi' class='nn_marker'>Ютел</span></p>
<span class='nn_light' id='marker_usi_text'>
После списания суммы покупки на вашем счете должно остаться не менее 30 руб.<br />
Минимальная сумма единовременного платежа 1 руб.<br />
Максимальный разовый платеж - 5000 руб.<br />
Максимальная сумма платежей за сутки - 15000 руб.<br />
Максимальная сумма платежей за месяц – 15000 руб.<br />
Максимальная количество платежей за сутки - 10<br />
Максимальная количество платежей за неделю - 20<br />
Максимальная количество платежей за месяц - 50<br />
<br />
«Мобильные платежи» доступны всем абонентам «МегаФона», за исключением юридических лиц и абонентов, обслуживающихся по кредитной системе расчетов.
</span>
</div>
		</div>
		
	</div>
</div>