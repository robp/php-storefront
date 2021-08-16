<html>

<head>
<title><?php echo $config->store_name ?>: Checkout</title>
</head>

<body bgcolor=#ffffff>

<center>

<table border=0 width=80%>
<tr>
<td>
<font face="Arial, MS Sans Serif, Helvetica" size="-1">

<center>
<h1><?php echo $config->store_name ?></h1>
</center>

<h3>Checkout</h3>

<?php if ($errmsg) echo "<b>Your order could not be processed because of following reason:<br> <font color=#CC0000>$errmsg</font></b><p>"; ?>
