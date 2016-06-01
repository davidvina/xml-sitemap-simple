<?php

/**
 * Constructs a descriptive page.
 *
 * Our menu maps this function to the path 'examples/page_example'.
 */
function xml_sitemap_simple_admin_output_form($form, $form_state) {

  //xml_sitemap_simple_save_file('holita');



  $nodes_sitemap = get_values_sitemap();
  dpm($nodes_sitemap);

  //$terms_sitemap = get_values_sitemap('taxonomy_term');
  //dpm($terms_sitemap);

  // sample of render array
  // https://www.drupal.org/node/930760
  // http://themery.com/book/export/html/100
  // elements are liste on system_element_info
 

    /*
  $form = array(
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


    // botones
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create sitemap.xml file'),
    //'#submit' => array('xml_simple_sitemap_create_xml'),
  );

  return $form;
}


/**
 *  implement hook_form_submit()
 *
 *  @param array $form
 *  @param array $form_state
 */
function xml_sitemap_simple_admin_output_form_submit($form, $form_state){

  //create_node_sitemap();
}



function get_values_sitemap($entityType = 'node'){
    // setting data nodes
  $config_values = config_get('xml_sitemap_simple.settings', $entityType);

  $sitemap_array = array();  

  global $base_url;

  foreach ($config_values as $bundle => $value) {
    
    // create sitemap only for active node types
     if($value['active'] == 1){

        // take nodes from bundle
        switch ($entityType) {
          case 'node':
            $ids = query_id_nodes($bundle);
            break;
          
          case 'taxonomy_term':
            $ids = query_id_terms($bundle);
            break;
        }



        // get urls from nodes_ids
        foreach ($ids as $id) {  


            $excludeSitemap = 0;
            $priority = $value['priority'];
            $frequency = $value['frequency'];

            // look if there is a custom sitemap per node
            $custom_value = xml_sitemap_simple_get_record($entityType, $id);

            
            if(!empty($custom_value)){
              // if exist change values
              $excludeSitemap = $custom_value->exclude;
              if(!$excludeSitemap){
                $priority = $custom_value->priority;
                $frequency = $custom_value->frequency;
              }
            } 

            if(!$excludeSitemap){
              $sitemap_array[$id] = array(
                  'loc' => $base_url.'/'.backdrop_get_path_alias("node/".$id),
                  'lastmod' => get_last_modification($id),
                  'changefreq' => $frequency,
                  'priority' => $priority,
              );
            }
            
        }
     }
  }

  return $sitemap_array;

}






// funcion que devuelve las ids de nodos publicados
function query_id_nodes($bundle_type){

     $query = new EntityFieldQuery();
     $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', $bundle_type)
      ->propertyCondition('status', 1);
  
      $result = $query->execute();
      $result = $result['node'];

      $keys = array();
      $keys = array_keys($result);

      return $keys;
}


function query_id_terms($bundle_type){

     /*
     $query = new EntityFieldQuery();
     $query->entityCondition('entity_type', 'taxonomy_term')
      ->entityCondition('bundle', $bundle_type);
      //->propertyCondition('status', 1);
  
      $result = $query->execute();
      dpm($result);


      //$result = $result['node'];

      //$keys = array();
      //$keys = array_keys($result);

      return $keys;
      */
        $query = db_select('taxonomy_index', 't')
         // ->condition('tid', $tids, 'IN')

          //->join('node', 'n', 't.nid = n.nid')
          ->fields('t', array('tid', 'created'))
          //->condition('n.status', 1, '=')
          ->execute()
          ->fetchAssoc();
        //$query->condition('n.language', $langs, 'IN');

        dpm($query);

}





// funcion para sacar fecha última modificacion
function get_last_modification($id){

    // for nodes
    $result = db_select('node', 'n')
    ->fields('n', array('changed'))
    ->condition('n.nid', $id)
    ->execute()
    ->fetchAssoc();

    return date('Y-m-d', $result['changed']);
}


// funcion que devuelve las url de los nodos
function xml_sitemap_node_url($node_type){
 
  $nodeUrlAlias = array();
  $node_ids = query_id_nodes($node_type);

  global $base_url;


  // loop over node ids
  foreach ($node_ids as $key) {
    $nodeUrlAlias[] = drupal_get_path_alias($base_url."/node/".$key);
  }

  return $nodeUrlAlias;

}





