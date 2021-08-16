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

  $filename = "cache/cat" . $id . ".html";
  $use_cache = 0;

  // If the cached file exists, then use the cached copy
  // to save big resources
  if (USE_CACHEING && file_exists($filename)) {
    $stats = stat($filename);

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
    if (!$missing_info) {
      // If we were passed the category id and the category actually
      // has a template assigned to it, then continue
      $category = new Category (0, $id);

      if ($category->template_id) {
        $template = new Template (0, $category->template_id);

        $tmpl_url = $config->template_url . $template->filename;

        $file = fopen($tmpl_url, "r");

        if ($file) {
          // Read the whole template file into $result
          while (!feof($file)) {
            $result .= fgets($file, 1024);
          }

          fclose($file);

          $result_array = split ("(%BEGIN_LOOP%|%END_LOOP%)", $result);
          $result = parse_dynamic_page($result_array[0], 0, 0, $category->id);

          if (!$subcats = $category->get_subs()) {
            $query = "SELECT * FROM items WHERE category = " . $category->id . "
                      ORDER BY name";

            if (!$sql_result = mysql_query($query, $sock)) {
              echo mysql_error();
              html_exit();
            }

            for ($a = 0; $a < mysql_num_rows($sql_result); $a++) {
              $item = new Item(mysql_fetch_row($sql_result), 0);
              if ($item->id != $last_id)
                $items[] = $item;
              $last_id = $item->id;
            }
          }

          if ($subcats)
            $num_items = count($subcats);
          else
            $num_items = count($items);

          $items_per_row = 1;

          $inner_array = split ("(%BEGIN_INNER_LOOP\([0-9]+\)%|%END_INNER_LOOP%)", $result_array[1]);

          if (count($inner_array) == 3) {
            ereg("%BEGIN_INNER_LOOP\(([0-9]+)\)%", $result_array[1], $regs);
            $inner_loop = 1;
            $items_per_row = $regs[1];
          }

          if ($inner_loop) {
            $num_rows = $num_items / $items_per_row;
            $num_rows += ($num_items % $inner_loop_count ? 1 : 0);
          }
          else
            $num_rows = $num_items;

          for ($a = 0; $a < $num_rows; $a++) {
            for ($b = 0; $b < $items_per_row; $b++) {
              $item_number = ($a * $items_per_row) + $b;

              if (($item_number + 1) > $num_items)
                break;

              if ($subcats)
                $item = $subcats[$item_number];
              else
                $item = $items[$item_number];

              if ($inner_loop) {
                $result_loop = $inner_array[1];
                if ($b == 0)
                  $result .= parse_dynamic_page($inner_array[0], 0, 0, 0);
              }
              else
                $result_loop = $result_array[1];

              $result_loop = str_replace("%LOOP_ID%", $item_number + 1, $result_loop);
              $result_loop = str_replace("%INNER_LOOP_ID%", $b + 1, $result_loop);

              if ($subcats)
                $result_loop = parse_dynamic_page($result_loop, 0, 0, $item->id);
              else
                $result_loop = parse_dynamic_page($result_loop, 0, $item->sku, 0);

              $result .= $result_loop;

              if ($inner_loop && $b == ($items_per_row - 1))
                $result .= parse_dynamic_page($inner_array[2], 0, 0, 0);
            }
          }

          $result .= parse_dynamic_page($result_array[2], 0, 0, $category->id);

          // Write the cache file
          if (USE_CACHEING && ($file = fopen($filename, "w"))) {
            fwrite($file, $result, strlen($result));
            fclose($file);
          }
        }
        else {
          $errmsg = "Unable to open template.";
        }
      }
      else {
        $errmsg = "Category is not dynamic.";
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
