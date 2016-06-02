<?php

function xml_sitemap_simple_output(){

  $data = load_sitemap_xml_file();
  
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

  $output = $data;
  print $output;

  backdrop_exit();
  
}


function load_sitemap_xml_file(){
  $uri = 'public://sitemap.xml';

  if (!is_file($uri)) {
    backdrop_set_message(t('The file %uri does not exist', array('%uri' => $uri)), 'error');
    return;
  }

  return file_get_contents($uri);

}