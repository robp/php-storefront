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
  $required1 = array("id");

  // Check to see that all required form inputs are found
  for ($i = 0; $i < sizeof($required1); $i++) {
    if (!$$required1[$i]) {
      $missing_info = 1;
      break;
    }
  }

  $filename = "cache/prod" . $id . ".html";
  $use_cache = 0;

  // If the cached file exists, then use the cached copy
  // to save big resources
  if (USE_CACHEING && file_exists($filename)) {
    $stats = stat($filename);

    // If the file's date is the same as today's date, then
    // it's safe to use the cached copy, since any dynamic
    // date elements in the file will be the same
    if (date("Ymd") == date("Ymd", $stats[9])) {
      if ($file = fopen($filename, "r")) {
        while (!feof($file)) {
          $result .= fgets($file, 1024);
        }

        fclose($file);
        $use_cache = 1;
      }
    }
  }

  if (!$use_cache) {
    // If we were passed the item id and the id is actually
    // in the dynamic items table, then continue.
    if (!$missing_info && db_item_exists("id", $id, "items_dynamic")) {
      $itemdyn = new ItemDyn (0, $id);
      $template = new Template (NULL, $itemdyn->template_id);

      if ($template->type == 1) {
        echo "Template specified is a category template.";
        exit();
      }

      $tmpl_url = $config->template_url . $template->filename;

      $file = fopen($tmpl_url, "r");
      if (!$file) {
        $errmsg = "Unable to open template.";
      }
      else {
        // Read the whole template file into $result
        while (!feof($file)) {
          $result .= fgets($file, 1024);
        }

        fclose($file);

        $result = parse_dynamic_page($result, $itemdyn->id, 0, 0);

        // (Over)write the cache file
        if (USE_CACHEING && ($file = fopen($filename, "w"))) {
          fwrite($file, $result, strlen($result));
          fclose($file);
        }
      }
    }
    else {
      $errmsg = "No item id specified, or item is not dynamic.";
    }
  }

  if ($errmsg)
    echo $errmsg;
  else
    echo $result;
?>
