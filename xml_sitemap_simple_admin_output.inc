<?php

/**
 * Constructs a descriptive page.
 *
 * Our menu maps this function to the path 'examples/page_example'.
 */
function xml_sitemap_simple_admin_output_form($form, $form_state) {

  $form = array();

  $sitemap_data = array_merge(get_values_sitemap('node'), get_values_sitemap('taxonomy_term')); 
  
  // ordenamos array
  $sitemap_data = array_orderby($sitemap_data, 'priority', SORT_DESC, 'lastmod', SORT_DESC);


  $form['sitemap_wrapper'] = array(
    '#type' => 'markup',
    '#prefix' => '<ul>',
    '#suffix' => '</ul>',
  );


  foreach ($sitemap_data as $key => $value) {
    # code...
    //dpm($value);

    $form['sitemap_wrapper']['element'.$key] = array(
      '#type' => 'markup',
      '#prefix' => '<li>',
      '#markup' => '<p>loc: '.$value['loc'].'<br>'.'lastmod: '.$value['lastmod'].'<br>'.'changefreq: '.$value['changefreq'].'<br>'.'priority: '.$value['priority'].'<br></p>',
      '#suffix' => '</li>',
    );
  }


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

  create_sitemap();
  
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
            $path_base = 'node/';
            break;
          
          case 'taxonomy_term':
            $ids = query_id_terms($bundle);
            $path_base = 'taxonomy/term/';
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
              $sitemap_array[] = array(
                  'type' => $entityType,
                  'bundle' => $bundle,
                  'id' => $id,
                  'loc' => $base_url.'/'.backdrop_get_path_alias($path_base.$id),
                  'lastmod' => get_last_modification($id, $entityType),
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
     
    $query_all_terms = new EntityFieldQuery();
    $query_all_terms->entityCondition('entity_type', 'taxonomy_term')
      ->entityCondition('bundle', $bundle_type);

    $result_all_terms = $query_all_terms->execute();
    $result_all_terms = $result_all_terms['taxonomy_term'];

    // get all terms ids
    $tids = array_keys($result_all_terms);

    // look if term has associated nodes
    foreach ($tids as $tid) {
      # code...
      $node_terms = taxonomy_select_nodes($tid, FALSE, FALSE);
      // if hasn't associated nodes
      if(count($node_terms) == 0){
        unset($tids[$tid]);
      }

    }

    // only returns taxonomies ids with associated data
    return $tids;
      
}





// funcion para sacar fecha última modificacion
function get_last_modification($id, $entityType = 'node'){

  switch ($entityType) {
    case 'node':
        // for nodes
        $result = db_select('node', 'n')
        ->fields('n', array('changed'))
        ->condition('n.nid', $id)
        ->execute()
        ->fetchAssoc();

      break;

    case 'taxonomy_term':

      $query = db_select('node', 'n');
      $query->join('taxonomy_index', 't', 't.nid = n.nid');
      $query->fields('n', array('changed'));
      //$query->fields('t', array('tid'));
      $query->condition('t.tid',$id,'=');
      $query->orderBy('n.changed', 'DESC');
      $result = $query->execute()->fetchAssoc();

      break;
    
  }


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


/*
* Funcion para ordenar arrays
*/
function array_orderby() {
    $args = func_get_args();
    $data = array_shift($args);
    foreach ($args as $n => $field) {
        if (is_string($field)) {
            $tmp = array();
            foreach ($data as $key => $row)
                $tmp[$key] = $row[$field];
            $args[$n] = $tmp;
            }
    }
    $args[] = &$data;
    call_user_func_array('array_multisort', $args);
    return array_pop($args);
}

