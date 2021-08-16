Please take a moment to verify that your order is correct. If you would like to make any changes, you can return to the <a href="<?php echo $config->store_url ?>">Store Menu</a> or to <a href="<?php echo $config->viewbasket_url ?>?referer=<?php echo $referer ?>">Your Order</a>. All prices are in <b>Canadian dollars</b>.<p>

When you are ready to continue, please fill out the form below.<p>

<table border=0 cellpadding=0 cellspacing=0 width=100%>
<tr>
<td nowrap align=left width=180><font face="Arial, MS Sans Serif, Helvetica" size=1><b>Item</b></font></td>
<td nowrap align=left><font face="Arial, MS Sans Serif, Helvetica" size=1><b>Part No.</b></font></td>
<td nowrap align=right><font face="Arial, MS Sans Serif, Helvetica" size=1><b>Unit Price</b></font></td>
<td nowrap align=center><font face="Arial, MS Sans Serif, Helvetica" size=1><b>Qty.</b></font></td>
<td nowrap align=right><font face="Arial, MS Sans Serif, Helvetica" size=1><b>Price</b></font></td>
</tr>

<tr>
<td colspan=5><img height=1 vspace=4 width=100% border=0 alt="Image" align=middle src="img/pixelblack.gif"></td>
</tr>

<?php
  $discount = $order->calc_discount();

  for ($i = 0; $i < count($order->bundles); $i++) {
    $bundle = $order->bundles[$i];
    $bundle_price = 0;

    for ($j = 0; $j < count($bundle->items); $j++) {
      $bundle_item = $bundle->items[$j];
      $bundle_item_cost = $bundle_item->price * $bundle_item->quantity;
      $bundle_item_weight = $bundle_item->weight * $bundle_item->quantity;

      // Add the item's cost to the bundle price
      $bundle_price += $bundle_item_cost;
    }

    printf("
<tr>
<td colspan=2><font face=\"Arial, MS Sans Serif, Helvetica\" size=3><b>%s</b></font></td>
<td align=right><font face=\"Arial, MS Sans Serif, Helvetica\" size=1>$%s</font></td>
<td align=center><font face=\"Arial, MS Sans Serif, Helvetica\" size=1>%d</font></td>
<td align=right><font face=\"Arial, MS Sans Serif, Helvetica\" size=1><b>$%s</b></font></td>
</tr>\n",

       $bundle->name, 
       format_money($bundle_price),
       $bundle->quantity,
       format_money($bundle_price * $bundle->quantity));

    for ($j = 0; $j < count($bundle->items); $j++) {
      $bundle_item = $bundle->items[$j];

      printf("
<tr>
<td width=180><font face=\"Arial, MS Sans Serif, Helvetica\" size=1>&nbsp;&nbsp;&nbsp;%s</font></td>
<td valign=top align=left><font face=\"Arial, MS Sans Serif, Helvetica\" size=1>%s</font></td>
</tr>", $bundle_item->name, $bundle_item->sku);
    }

echo "
<tr>
<td colspan=5><img height=1 vspace=4 width=100% border=0 alt=\"Image\" align=middle src=\"img/pixelblack.gif\"></td>
</tr>
";
  }
?>

<?php if ($discount): ?>

<tr bgcolor=#e0e0e0>
<td colspan=4>
<font face="Arial, MS Sans Serif, Helvetica" size=2>
<b>Applicable discounts</b>
</font>
</td>
<td align=right>
<font face="Arial, MS Sans Serif, Helvetica" size=1>- $<?php echo format_money($discount) ?></font></td>
</tr>

<tr>
<td colspan=5><img height=1 vspace=4 width=100% border=0 alt="Image" align=middle src="img/pixelblack.gif"></td>
</tr>

<?php endif; ?>

<tr>
<td colspan=5 align=right>

  <table border=0 cellpadding=0 cellspacing=0>
  <tr>
  <td rowspan=3 width=10> </td>
  <td rowspan=3 valign=center>
  <font face="Arial, MS Sans Serif, Helvetica" size=1>
  Applicable shipping charges will be added to your total, as indicated in the "Shipping Method" section below.
  </td>
  <td rowspan=3 width=10> </td>

  <td align=right><font face="Arial, MS Sans Serif, Helvetica" size=1>PST&nbsp;&nbsp;$<?php echo format_money($pst_total) ?></font></td>
  </tr>
  <tr>
  <td align=right><font face="Arial, MS Sans Serif, Helvetica" size=1>GST&nbsp;&nbsp;$<?php echo format_money($gst_total) ?></font></td>
  </tr>
  <tr>
  <td align=right>

    <table border=0 cellpadding=0 cellspacing=0 bordercolor=#000000>
    <tr>
    <td nowrap><font face="Arial, MS Sans Serif, Helvetica" size=1><b>Order Total</b> <font size=4 color=#CC0000><b>$<?php echo format_money($total); ?></b></font></font></td>
    </tr>
    </table>

  </td>
  </tr>
  </table>

</td>
</tr>
</table>

<p>

<form action="checkout.phtml" method="post">
<input type=hidden name="step" value="2">
<input type=hidden name="referer" value="<?php echo $referer ?>">
<input type=hidden name="bill_first_name" value="<?php echo $bill_first_name ?>">
<input type=hidden name="bill_last_name" value="<?php echo $bill_last_name ?>">
<input type=hidden name="bill_email" value="<?php echo $bill_email ?>">
<input type=hidden name="bill_day_phone_area_code" value="<?php echo $bill_day_phone_area_code ?>">
<input type=hidden name="bill_day_phone_exchange" value="<?php echo $bill_day_phone_exchange ?>">
<input type=hidden name="bill_day_phone_number" value="<?php echo $bill_day_phone_number ?>">
<input type=hidden name="bill_eve_phone_area_code" value="<?php echo $bill_eve_phone_area_code ?>">
<input type=hidden name="bill_eve_phone_exchange" value="<?php echo $bill_eve_phone_exchange ?>">
<input type=hidden name="bill_eve_phone_number" value="<?php echo $bill_eve_phone_number ?>">
<input type=hidden name="bill_address_1" value="<?php echo $bill_address_1 ?>">
<input type=hidden name="bill_city" value="<?php echo $bill_city ?>">
<input type=hidden name="bill_state" value="<?php echo $bill_state ?>">
<input type=hidden name="bill_zipcode" value="<?php echo $bill_zipcode ?>">
<input type=hidden name="ship_first_name" value="<?php echo $ship_first_name ?>">
<input type=hidden name="ship_last_name" value="<?php echo $ship_last_name ?>">
<input type=hidden name="ship_day_phone_area_code" value="<?php echo $ship_day_phone_area_code ?>">
<input type=hidden name="ship_day_phone_exchange" value="<?php echo $ship_day_phone_exchange ?>">
<input type=hidden name="ship_day_phone_number" value="<?php echo $ship_day_phone_number ?>">
<input type=hidden name="ship_address_1" value="<?php echo $ship_address_1 ?>">
<input type=hidden name="ship_city" value="<?php echo $ship_city ?>">
<input type=hidden name="ship_state" value="<?php echo $ship_state ?>">
<input type=hidden name="ship_zipcode" value="<?php echo $ship_zipcode ?>">

<img height=1 hspace=0 vspace=4 width=350 border=0 alt=Image align=middle src="img/pixelblack.gif"><br>
<font size=3><b>Shipping Method</b></font><p>

<blockquote>

The cost of shipping will be added to your order total.<p>

<?php
  if (USE_SHIPPING) {
    echo "<select name=\"ship_method\">";

    for ($i = 0; $i < count($shiptypes_display); $i++) {
        printf("<option value=\"%d\"%s>%s: $%s",
               $shiptypes_display[$i]->id,
               ($ship_method == $shiptypes_display[$i]->id ? " selected" : ""), 
               $shiptypes_display[$i]->description,
               $shipcosts_display[$i]);
    }

    echo "</select>";
  }
?>

</blockquote>

<p>

<img height=1 hspace=0 vspace=4 width=350 border=0 alt=Image align=middle src="img/pixelblack.gif"><br>
<font size=3><b>Payment Information</b></font><p>

<blockquote>

<table border=0 cellpadding=0 cellspacing=0>
<tr>
<td colspan=5 height=15>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Card Type
</font></td>
</tr>
<tr>

<td align=left><input type=radio value="mastercard" name="cc_type"></td>
<td><img height=24 hspace=0 vspace=0 width=38 border=0 alt=Image align=middle src="img/creditcardmastercard.gif"></td>
<td width=15></td>

<td><input type=radio value="visa" name="cc_type" checked></td>
<td><img height=24 hspace=0 vspace=0 width=38 border=0 alt=Image align=middle src="img/creditcardvisa.gif"></td>
</tr>
</table>

<p>

<table border=0 cellpadding=0 cellspacing=0>
<tr>
<td height=15>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Card Number
</font></td>
<td width=15></td>
<td height=15 colspan=3>
<font face="Arial, MS Sans Serif, Helvetica" size=1>
                Expiration Date
</font></td>
</tr>
<tr>
<td><input maxlength=16 size=16 type=text name="cc_number" value="4111111111111111"></td>
<td width=15></td>

<td><select name="cc_exp_month">
<option value="01">January
<option value="02">February
<option value="03">March
<option value="04">April
<option value="05">May
<option value="06">June
<option value="07">July
<option value="08">August
<option value="09">September
<option value="10">October
<option value="11">November
<option value="12">December
</select></td>
<td width=5></td>
<td><select name="cc_exp_year">
<option value="1999">1999
<option value="2000">2000
<option value="2001">2001
<option value="2002">2002
<option value="2003" selected>2003
<option value="2004">2004
<option value="2005">2005
<option value="2006">2006
<option value="2007">2007
</select></td>
</tr>
</table>

<p>

<font size=1>
Note: <?php echo $config->store_name ?> uses industry-standard SSL (Secure Sockets Layer) and Triple-DES (Data Encryption Standard) encryption to protect the confidentiality of your personal information.
</font>

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
