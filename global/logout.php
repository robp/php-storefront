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

  $success = 0;

  if (!strlen($source))
    $source = getenv("HTTP_REFERER");
  
  if (!strlen($source))
    $source = $config->store_url;

  // In this case, referer is used so that the source script can
  // pass the original referer to the next script, such as in the
  // checkout area, or viewbasket area.
  if (!strlen($referer))
    $referer = $source;

  // The url variable can contain a URL that displays a confirmation 
  // page when the user logs in successfully.  If this isn't set, then
  // we just go back to the page they came from (source).
  if (!strlen($url))
    $url = $source;

  // Make sure the user can't pass their own uid variable.
  if (isset($uid))
    unset($uid);

  // Grab the uid session variable, if it exists.
  session_register("uid");

  if (isset($uid)) {
    $user = new User(0, 0, $uid);
    if ($user->id) {
      session_destroy();
      $success = 1;
    }
  }

  if (!$success) {
    $errmsg = urlencode("Logout incorrect.");
    header("Location: " . $source . "?referer=" . $referer . "&msg=" . $errmsg . "\n\n");
  }
  else {
    $errmsg = urlencode("Logout successful.");
    header("Location: " . $url . "?referer=" . $referer . "&msg=" . $errmsg . "\n\n");
  }
?>
