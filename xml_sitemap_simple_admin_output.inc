<?php

/**
 * Constructs a descriptive page.
 *
 * Our menu maps this function to the path 'examples/page_example'.
 */
function sitemap_admin_output() {

  // sample of render array
  // https://www.drupal.org/node/930760
  // http://themery.com/book/export/html/100
  // elements are liste on system_element_info
  /*
  $output = array(
    'first_para' => array(
      '#type' => 'markup',
      '#prefix' => 'hola',
      '#markup' => 'A paragraph about some stuff...',
      '#suffix' => 'adios',
      '#theme_wrappers' => 'lala',
    ),
    'second_para' => array(
      '#items' => array('first item', 'second item', 'third item'),
      '#theme' => 'item_list',
    ),
  );

  $page['highlighted']['new_stuff'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => 'my-container'),
  );
  
  $page['highlighted']['new_stuff']['heading'] = array(
    '#type' => 'head_tag',
    '#tag' => 'h2',
    '#value' => t('Heading'),
    '#attributes' => array('id' => 'my-heading'),
  );


  $page['highlighted']['new_stuff']['list'] = array(
    '#theme' => 'item_list',
    '#items' => array(
      'First item',
      'Second item',
      'Third item',
    ),
    '#attributes' => array('id' => 'my-heading', 'class' => 'hola'), 
  );
  */



  $url_list = xml_sitemap_node_url('page');

  $page['url_list'] = array(
    '#items' => $url_list,
    '#theme' => 'item_list',
  );

  return $page;
}