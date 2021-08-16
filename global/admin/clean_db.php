#!/usr/local/bin/php -q
<?php
  // Get some configuration constants and functions
  require("../inc/global_config.inc");
  require("../inc/config.inc");
  require("../inc/classes.inc");
  require("../inc/functions.inc");
  require("../inc/html.inc");

  // Connect to the SQL server and select the database
  $sock = db_connect();
  $config = new Config();

  $query = "SELECT basket_id FROM orders
            WHERE expiry < " . time();

  if (!$sql_result = mysql_query($query, $sock)) {
    echo mysql_error();
    exit();
  }

  for ($i = 0; $i < mysql_num_rows($sql_result); $i++) {
    $sql_result_row = mysql_fetch_row($sql_result);
    $order = new Order();
    $order->get_by_basket_id($sql_result_row[0]);
    $order->delete();
  }

  echo "$i old orders deleted.\n";
?>
