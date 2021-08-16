<?php
  function html_exit () {
    html_tail();
    exit();
  }

  function html_head () {
    global $config;

    echo "
<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">
<html>

<body bgcolor=#FFFFFF background=\"img/bg-body.gif\" marginwidth=0 marginheight=0 topmargin=0 leftmargin=0>
" . FONT1 . "
<blockquote>
<br>
";
  }

  function html_tail () {
    echo "
<p>

</blockquote>

<br>

<table border=0 cellpadding=10 cellspacing=0 width=100%>
<tr bgcolor=#000000>
<td valign=top>
" . FONT1 . "<font color=#ffffff>
<i>
" . PROGRAM_NAME . " version " . PROGRAM_VERSION . "<br>
<font size=1>Updated: " . PROGRAM_UPDATED . "</font>
</i>
</font></font>
</td>

<td align=right valign=top>
" . FONT1 . "<font color=#ffffff>
<i>
<a href=\"mailto:" . PROGRAM_ADMIN_EMAIL . "\"><font color=Green><b>" . PROGRAM_ADMIN_EMAIL . "</b></font></a>
</i>
</font></font>
</td>
</table>

</body>
</html>
";
  }

  function ad_display($id, $secure, $type) {
    $url = "http://www.wincom.net/AdServ/display.phtml?id=" . $id;

    if ($secure)
      $url .= "&secure=1";

    $adfile = fopen($url, "r");

    if (!$adfile) {
      $result = "Unable to open ad display.";
    }
    else {
      while (!feof($adfile)) {
        $result .= fgets($adfile, 1024);
      }
    }

    if ($type == 1)
      return $result;
    else
      echo $result;
  }
?>
