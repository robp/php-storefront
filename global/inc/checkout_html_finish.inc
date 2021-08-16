<b>Order #:</b> <?php echo $order_id ?><br>
<b>Auth. #:</b> <?php echo $transact->authno ?><p>

Thank you for shopping at <?php echo $config->store_name ?>! You should receive an electronic receipt in your email shortly. If you have any questions or comments about your order, please notify us at your convenience.<p>

<a href="<?php echo $referer ?>"><img width=148 height=26 border=0 alt="Continue Shopping" src="img/buttoncontinueshopping.gif"></a><p>

Here is a copy of your electronic receipt. We recommend printing (or saving) this page for future reference.<p>

<font face="Courier New, Courier" size="-1">
<pre>
<?php echo $msg ?>
</pre>
</font>
