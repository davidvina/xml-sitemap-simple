<?php

function xml_sitemap_simple_output(){
  //$output = 'hola';

  $headers = array(
    'Content-type' => 'text/xml; charset=utf-8',
    //'Content-length' => filesize($file),
    'Last-modified' => '$last_modified',
    //'Etag' => '$etag',
    //'Expires' => 'gmdate(DATE_RFC1123, $mtime + variable_get( 0))',
    //'Cache-Control' => 'must-revalidate',
    'X-Robots-Tag' => 'noindex, follow',
  );

  foreach ($headers as $name => $value) {
    backdrop_add_http_header($name, $value);
  } 

  $output.= "<?xml version='1.0' encoding='UTF-8' ?>\n";
  $output.= " <urlset xmlns='http://www.sitemaps.org/schemas/sitemap/0.9'>\n";
  $output.= "   <url>\n";
  $output.= "   <loc>http://localhost/hdcvi_ok/</loc>\n";
  $output.= "   <lastmod>2004-12-23T18:00:15+00:00</lastmod>\n";
  $outpur.= "   <changefreq></changefreq>\n";
  $output.= "   <priority>0.3</priority>\n";
  $output.= "   </url>\n";
  $output.= " </urlset>\n";

 // return $output;
  print $output;
  backdrop_exit();

}
