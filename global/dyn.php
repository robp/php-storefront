<?php
  // Get some configuration constants and functions
  require("inc/global_config.inc");
  require("inc/config.inc");
  require("inc/classes.inc");
  require("inc/functions.inc");
  require("inc/html.inc");

  // Connect to the SQL server and select the database
  $sock = db_connect();
  $config = new Config();

  // Setup which form inputs are required
  $required1 = array("f");

  // Check to see that all required form inputs are found
  for ($i = 0; $i < sizeof($required1); $i++) {
    if (!$$required1[$i]) {
      $missing_info = 1;
      break;
    }
  }


  // If we were passed a filename, continue
  if (!$missing_info) {
    $url = $config->store_url . $f;
    $file = fopen($url, "r");

    if ($file) {
      // Read the whole template file into $result
      while (!feof($file)) {
        $result .= fgets($file, 1024);
      }

      // Gotta fix this ad_display crap
      //$foo = ad_display(AD_LOC_ID, 0, 1);
      //$result = str_replace("%AD_DISPLAY%", $foo);

      $result = parse_dynamic_page($result, 0, 0, 0);
    }
    else {
      $errmsg = "Unable to open dynamic file.";
    }
  }
  else {
    $errmsg = "No dynamic file specified.";
  }

  if ($errmsg)
    echo $errmsg;
  else
    echo $result;
?>
