We respect your right to privacy, and will not disclose or sell information about you or your purchases to any outside organization for solicitation purposes without your consent.<p>

When you are ready to continue, please fill out the form below.<p>

<form action="checkout.phtml" method="post">
<input type=hidden name="step" value="1">
<input type=hidden name="referer" value="<?php echo $referer ?>">

<img height=1 hspace=0 vspace=4 width=350 border=0 alt=Image align=middle src="img/pixelblack.gif"><br>
<font size=3><b>Billing Information</b></font><p>

<blockquote>

<table border=0 cellpadding=0 cellspacing=0>
<tr>
<td height=15>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                First Name
</font></td>
<td width=5 rowspan=2></td>
<td height=15>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Last Name
</font></td>

<tr>
<td><input maxlength=14 size=14 type=text name="bill_first_name" value="<?php echo $bill_first_name ?>"></td>
<td><input maxlength=20 size=20 type=text name="bill_last_name" value="<?php echo $bill_last_name ?>"></td>
</tr>
</table>


<table border=0 cellpadding=0 cellspacing=0>
<tr>
<td height=15>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Address
</font></td>
</tr>
<tr>
<td><input maxlength=30 size=30 type=text name="bill_address_1" value="<?php echo $bill_address_1 ?>"></td>
</tr>
<tr>
<td height=2></td>
</tr>
<tr>
<td><input maxlength=30 size=30 type=text name="bill_address_2" value="<?php echo $bill_address_2 ?>"></td>
</tr>
</table>

<table border=0 cellpadding=0 cellspacing=0>
<tr>
<td height=15>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                City
</font></td>
<td width=5 height=2></td>
<td align=center>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Prov./State
</font></td>
<td width=5 height=2></td>
<td height=15>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Postal/Zip
</font></td>
</tr>

<tr>
<td><input maxlength=20 size=20 type=text name="bill_city" value="<?php echo $bill_city ?>"></td>
<td width=5 height=2></td>
<td align=center>
<select name="bill_state"><option value=""> &nbsp;
<?php
  //if (!$bill_state) $bill_state = $config->shipping_origin;
  for ($i = 0; $i < count($PROVINCES); $i++) {
    $state = $PROVINCES[$i];
    echo "<option value=\"" . $state[0] . "\"" . ($state[0] == $bill_state ? " selected" : "") . ">" . $state[0];
  }
  echo "<option value=\"\">--";
  for ($i = 0; $i < count($STATES); $i++) {
    $state = $STATES[$i];
    echo "<option value=\"" . $state[0] . "\"" . ($state[0] == $bill_state ? " selected" : "") . ">" . $state[0];
  }
?>
</select>
</td>
<td width=5 height=2></td>
<td><input maxlength=6 size=6 type=text name="bill_zipcode" value="<?php echo $bill_zipcode ?>"></td>
</tr>
</table>


<table border=0 cellpadding=0 cellspacing=0>
<tr>
<td height=15 colspan=6>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Daytime Phone
</font></td>
<td rowspan=2 width=10></td>
<td colspan=6>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Evening Phone
</font></td>
</tr>

<tr>
<td><font face="Arial, MS Sans Serif, Helvetica" size=1>(</font></td>
<td><input maxlength=3 size=3 type=text name="bill_day_phone_area_code" value="<?php echo $bill_day_phone_area_code ?>"></td>
<td><font face="Arial, MS Sans Serif, Helvetica" size=1>)</font></td>
<td><input maxlength=3 size=3 type=text name="bill_day_phone_exchange" value="<?php echo $bill_day_phone_exchange ?>"></td>
<td><font face="Arial, MS Sans Serif, Helvetica" size=1>-</font></td>
<td><input maxlength=4 size=4 type=text name="bill_day_phone_number" value="<?php echo $bill_day_phone_number ?>"></td>

<td><font face="Arial, MS Sans Serif, Helvetica" size=1>(</font></td>
<td><input maxlength=3 size=3 type=text name="bill_eve_phone_area_code" value="<?php echo $bill_eve_phone_area_code ?>"></td>
<td><font face="Arial, MS Sans Serif, Helvetica" size=1>)</font></td>
<td><input maxlength=3 size=3 type=text name="bill_eve_phone_exchange" value="<?php echo $bill_eve_phone_exchange ?>"></td>
<td><font face="Arial, MS Sans Serif, Helvetica" size=1>-</font></td>
<td><input maxlength=4 size=4 type=text name="bill_eve_phone_number" value="<?php echo $bill_eve_phone_number ?>"></td>
</tr>
</table> 

<table border=0 cellpadding=0 cellspacing=0>
<tr>
<td height=15 colspan=6>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Email Address
</font></td>
<td rowspan=2 width=15></td>
</tr>
<tr>
<td><input maxlength=50 size=31 type=text name="bill_email" value="<?php echo $bill_email ?>"></td>
</tr>
</table>

<p>

<font size=1>Please double-check your email address to be sure you<br>
receive your electronic receipt.</font>

</blockquote>

<p>

<img height=1 hspace=0 vspace=4 width=350 border=0 alt=Image align=middle src="img/pixelblack.gif"><br>
<font size=3><b>Shipping Information</b></font><p>

<blockquote>

<input type=checkbox name="ship_same_as_bill" value=1<?php if ($ship_same_as_bill == "1") echo " checked"; ?>> Check here if same as Billing Information.<p>

<table border=0 cellpadding=0 cellspacing=0>
<tr>
<td height=15>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                First Name
</font></td>
<td width=5 rowspan=2></td>
<td height=15>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Last Name
</font></td>

<tr>
<td><input maxlength=14 size=14 type=text name="ship_first_name" value="<?php echo $ship_first_name ?>"></td>
<td><input maxlength=20 size=20 type=text name="ship_last_name" value="<?php echo $ship_last_name ?>"></td>
</tr>
</table>


<table border=0 cellpadding=0 cellspacing=0>
<tr>
<td height=15>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Address
</font></td>
</tr>
<tr>
<td><input maxlength=30 size=30 type=text name="ship_address_1" value="<?php echo $ship_address_1 ?>"></td>
</tr>
<tr>
<td height=2></td>
</tr>
<tr>
<td><input maxlength=30 size=30 type=text name="ship_address_2" value="<?php echo $ship_address_2 ?>"></td>
</tr>
</table>

<table border=0 cellpadding=0 cellspacing=0>
<tr>
<td height=15>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                City
</font></td>
<td width=5 height=2></td>
<td align=center>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Prov./State
</font></td>
<td width=5 height=2></td>
<td height=15>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Postal/Zip
</font></td>
</tr>

<tr>
<td><input maxlength=20 size=20 type=text name="ship_city" value="<?php echo $ship_city ?>"></td>
<td width=5 height=2></td>
<td align=center>
<select name="ship_state"><option value=""> &nbsp;
<?php
  //if (!$ship_state) $ship_state = $config->shipping_origin;
  for ($i = 0; $i < count($PROVINCES); $i++) {
    $state = $PROVINCES[$i];
    echo "<option value=\"" . $state[0] . "\"" . ($state[0] == $ship_state ? " selected" : "") . ">" . $state[0];
  }
  echo "<option value=\"\">--";
  for ($i = 0; $i < count($STATES); $i++) {
    $state = $STATES[$i];
    echo "<option value=\"" . $state[0] . "\"" . ($state[0] == $ship_state ? " selected" : "") . ">" . $state[0];
  }
?>
</select>
</td>
<td width=5 height=2></td>
<td><input maxlength=6 size=6 type=text name="ship_zipcode" value="<?php echo $ship_zipcode ?>"></td>
</tr>
</table>


<table border=0 cellpadding=0 cellspacing=0>
<tr>
<td height=15 colspan=6>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Daytime Phone
</font></td>
</tr>

<tr>
<td><font face="Arial, MS Sans Serif, Helvetica" size=1>(</font></td>
<td><input maxlength=3 size=3 type=text name="ship_day_phone_area_code" value="<?php echo $ship_day_phone_area_code ?>"></td>
<td><font face="Arial, MS Sans Serif, Helvetica" size=1>)</font></td>
<td><input maxlength=3 size=3 type=text name="ship_day_phone_exchange" value="<?php echo $ship_day_phone_exchange ?>"></td>
<td><font face="Arial, MS Sans Serif, Helvetica" size=1>-</font></td>
<td><input maxlength=4 size=4 type=text name="ship_day_phone_number" value="<?php echo $ship_day_phone_number ?>"></td>
</tr>
</table> 

</blockquote>

<p>

<table border=0 width=100% cellpadding=0 cellspacing=0>
<tr><td align=right>
<font face="Arial, MS Sans Serif, Helvetica" size=2>
<input width=86 border=0 height=26 alt=Continue type=image name="Continue" src="img/buttoncontinue.gif">
</font>
</td></tr>
</table>

</form>
