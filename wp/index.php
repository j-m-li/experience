<!DOCTYPE html>
<html lang="en">
 <head>
<!-- The authors disclaim copyright to this source code -->
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Dissident Morse With PHP</title>
  <link rel="stylesheet" href="wp-admin/css/style.css">
  <link rel="icon" type="image/x-icon" href="favicon.png">
 </head>
 <body>
     <h1>Chat by Morse</h1>
     <h1><img alt="" src="logo.svg"></h1>
     <div id="txt" style="word-wrap: break-word;white-space: pre-wrap;width:100%; max-width:100%; height:240px; max-height:240px;"></div>
     <div style="position:fixed;bottom:0px;width:100%;text-align:center;"
          ><button style="width:33%; height:200px;" id="short" onclick="cb(this.id, window.event.type, 0, 0)">.</button
          ><button style="width:33%; height:200px;" id="menu" onclick="cb(this.id, window.event.type, 0, 0)">[ ]</button
          ><button style="width:33%; height:200px;" id="long" onclick="cb(this.id, window.event.type, 0, 0)">_</button
          ></div>
     <script src="wp-admin/js/script.js" ></script>
 </body>
</html>

