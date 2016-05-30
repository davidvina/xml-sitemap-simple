<?php

/**
 *  settings for xml_sitemap_simple Nodes
 *
 *  @return array form API
 *  @param array $form
 *  @param array $form_state
 */

function xml_sitemap_simple_admin_node_form($form, &$form_state){

  // setting data nodes
  $config_nodes = config_get('xml_sitemap_simple.settings', 'nodes');

 	$form = array();

 	$form['description'] = array(
    '#prefix' => '<div class="description">',
 		'#markup' => '<p>'.t('Settings for node types').'<br><em>'.t('Select wich node types for include in the sitemap.xml file').'</em></p>',
    '#suffix' => '</div>',
 	);

  // extraemos listado de tipos de nodos
  $node_types = query_node_types_name();
  // loop a traves de todos los tipos de nodos
 	foreach ($node_types as $node_type) {

		// valores por defecto
    if(isset($config_nodes[$node_type])){
      $valorActiveDefault = $config_nodes[$node_type]['active'];
      $valorPriorityDefault = $config_nodes[$node_type]['priority'];
      $valorFrequencyDefault = $config_nodes[$node_type]['frequency'];
    }

    // construimos el formulario
    $form[$node_type."_active"] = create_checkbox_element($node_type, $valorActiveDefault);		
		$form[$node_type."_priority"] = create_number_element($node_type."_active", $valorPriorityDefault);
    $form[$node_type."_frequency"] = create_frequency_element($node_type."_active", $valorFrequencyDefault);

 	};


  // botones
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save nodes settings'),
  );

  return $form;
 }


/**
 *  settings for xml_sitemap_simple Terms by Vocabularies
 *
 *  @return array form API
 *  @param array $form
 *  @param array $form_state
 */

function xml_sitemap_simple_admin_terms_form($form, &$form_state){

  // setting data nodes
  $config_vocabularies = config_get('xml_sitemap_simple.settings', 'vocabularies');

  $form = array();
  $form['description'] = array(
    '#prefix' => '<div class="description">',
    '#markup' => '<p>'.t('Settings for vocabularies').'<br><em>'.t('Select wich taxonomy pages for include in the sitemap.xml file').'</em></p>',
    '#suffix' => '</div>',
  );


  // vocabularies

  $vocabularios = taxonomy_vocabulary_get_names();
  
  foreach ($vocabularios as $vocabulario) {

    
    // valores por defecto
    if(isset($config_vocabularies[$vocabulario->machine_name])){
      $valorActiveDefault = $config_vocabularies[$vocabulario->machine_name]['active'];
      $valorPriorityDefault = $config_vocabularies[$vocabulario->machine_name]['priority'];
      $valorFrequencyDefault = $config_vocabularies[$vocabulario->machine_name]['frequency'];
    }

    // construimos el formulario
    $form[$vocabulario->machine_name."_active"] = create_checkbox_element($vocabulario->machine_name, $valorActiveDefault);   
    $form[$vocabulario->machine_name."_priority"] = create_number_element($vocabulario->machine_name."_active", $valorPriorityDefault);
    $form[$vocabulario->machine_name."_frequency"] = create_frequency_element($vocabulario->machine_name."_active", $valorFrequencyDefault);

  }

  // botones
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save vocabularies settings'),
  );

  return $form;
}


/**
 *  settings for xml_sitemap_simple Views
 *
 *  @return array form API
 *  @param array $form
 *  @param array $form_state
 */
function xml_sitemap_simple_admin_views_form($form, &$form_state){

  // setting data nodes
  $config_views = config_get('xml_sitemap_simple.settings', 'views');

  dpm($config_views);

  $views_pages = xml_sitemap_simple_get_views_pages();
 
  $form = array();
  $form['description'] = array(
    '#prefix' => '<div class="description">',
    '#markup' => '<p>'.t('Settings for views').'<br><em>'.t('Select wich page view for include in the sitemap.xml file').'</em></p>',
    '#suffix' => '</div>',
  );


  foreach ($views_pages as $view_page) {
 
    foreach ($view_page as $name_view => $url_view) {

          // valores por defecto
      if(isset($config_views[$name_view])){
        $valorActiveDefault = $config_views[$name_view]['active'];
        $valorPriorityDefault = $config_views[$name_view]['priority'];
        $valorFrequencyDefault = $config_views[$name_view]['frequency'];
      }
 
      $form[$name_view."_active"] = array(
        '#type' => 'checkbox',
        '#title' => $name_view,
        '#default_value' => $valorActiveDefault,
        '#description' => t('Url').": <em>".$url_view."</em>",
      );
      $form[$name_view."_priority"] = create_number_element($name_view."_active", $valorPriorityDefault);
      $form[$name_view."_frequency"] = create_frequency_element($name_view."_active", $valorFrequencyDefault);

    }

  }

  // botones
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save views settings'),
  );

  return $form;
}



/**
 *  implement hook_form_submit()
 *
 *  @param array $form
 *  @param array $form_state
 */

function xml_sitemap_simple_admin_node_form_submit($form, $form_state){

	$values = $form_state['values'];

	// carga el axivo de configuracion
	$config = config('xml_sitemap_simple.settings');

  // array of nodes with data
  $nodeData = array();
  
  // loop trought node types
  $nodetypes = query_node_types_name();

  foreach ($nodetypes as $nodetype) {
    
    $dataToSave = array();

    foreach ($values as $key => $value) { 
      if(strpos($key, $nodetype) !== false){
      
        if(strpos($key, 'active') !== false){
          $dataToSave['active'] = $value;
        }       

        if(strpos($key, 'priority') !== false){
          $dataToSave['priority'] = $value;
        }

        if(strpos($key, 'frequency') !== false){
          $dataToSave['frequency'] = $value;
        }        
      }
    }
    $nodeData[$nodetype] = $dataToSave;
  }

  $config->set('nodes', $nodeData);
  $config->save();
}


/**
 *  implement hook_form_submit()
 *
 *  @param array $form
 *  @param array $form_state
 */
function xml_sitemap_simple_admin_terms_form_submit($form, &$form_state){

  $values = $form_state['values'];

  // carga el axivo de configuracion
  $config = config('xml_sitemap_simple.settings');

  // array of nodes with data
  $vocabularyData = array();
  
  // loop trought node types
  $vocabularies = taxonomy_vocabulary_get_names();

  foreach ($vocabularies as $vocabulary) {

    $dataToSave = array();

    foreach ($values as $key => $value) {
      if(strpos($key, $vocabulary->machine_name) !== false){
      
        if(strpos($key, 'active') !== false){
          $dataToSave['active'] = $value;
        }       

        if(strpos($key, 'priority') !== false){
          $dataToSave['priority'] = $value;
        }

        if(strpos($key, 'frequency') !== false){
          $dataToSave['frequency'] = $value;
        }        
      }
    }

    $vocabularyData[$vocabulary->machine_name] = $dataToSave;
  }

  $config->set('vocabularies', $vocabularyData);
  $config->save();
}



/**
 *  implement hook_form_submit()
 *
 *  @param array $form
 *  @param array $form_state
 */
function xml_sitemap_simple_admin_views_form_submit($form, &$form_state){

  dpm($form_state['values']);

  
  $values = $form_state['values'];

  // carga el axivo de configuracion
  $config = config('xml_sitemap_simple.settings');

  // array of nodes with data
  $viewsData = array();
   
  // loop trought node types
  $views = xml_sitemap_simple_get_views_names();

  foreach ($views as $view_name) {

    $dataToSave = array();

    foreach ($values as $key => $value) {
      if(strpos($key, $view_name) !== false){
      
        if(strpos($key, 'active') !== false){
          $dataToSave['active'] = $value;
        }       

        if(strpos($key, 'priority') !== false){
          $dataToSave['priority'] = $value;
        }

        if(strpos($key, 'frequency') !== false){
          $dataToSave['frequency'] = $value;
        }        
      }
    }

    $viewsData[$view_name] = $dataToSave;
  }

  $config->set('views', $viewsData);
  $config->save();

}



/**
 * This is Example 2, a basic form with a submit button.
 *
 * @see http://DOCUMENTATION_PENDING/node/717726
 * @ingroup xml_sitemap_simple
 */
/*
function xml_sitemap_simple_admin_menus_form($form, &$form_state){

  $config = config('xml_sitemap_simple.settings');

  $form = array();
  $form['description'] = array(
    '#prefix' => '<div>',
    '#markup' => 'Menus',
    '#suffix' => '</div>',
  );

  // menus

  $menus = menu_get_menus();
  unset($menus['management']);


  foreach ($menus as $menuID => $menuName) {

    $valorActiveDefault = $config->get("active_menu_".$menuID);
    $form ["active_menu_".$menuID] = create_checkbox_element($menuName, $valorActiveDefault);

    $valorPriorityDefault = $config->get("priority_menu_".$menuID);
    $form["priority_menu_".$menuID] = create_number_element($valorPriorityDefault, 'active_menu_'.$menuID);
  }

  // botones
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save menus settings'),
  );

  return $form;
}
*/





/**
 * This is Example 2, a basic form with a submit button.
 *
 * @see http://DOCUMENTATION_PENDING/node/717726
 * @ingroup xml_sitemap_simple
 */
/*
function xml_sitemap_simple_admin_custom_form($form, &$form_state){
  // We will have many fields with the same name, so we need to be able to
  // access the form hierarchically.
  $form['#tree'] = TRUE;


  $valorCustomUrl = $config->get('custom_url_sitemap');

  if (empty($form_state['num_custom_url_sitemap'])) {
    $form_state['num_custom_url_sitemap'] = 1;
  }



  // Build the number of name fieldsets indicated by $form_state['num_custom_url_sitemap']
  for ($i = 1; $i <= $form_state['num_custom_url_sitemap']; $i++) {
    $form['custom_url_sitemap'][$i] = array(
      '#type' => 'fieldset',
      '#title' => t('Custom url on sitemap #@num', array('@num' => $i)),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['custom_url_sitemap'][$i]['url'] = array(
      '#type' => 'textfield',
      '#description' => t("Enter URL"),
      '#size' => 20,
      '#maxlength' => 20,
      //'#required' => TRUE,
    );

    $form['custom_url_sitemap'][$i]['priority'] = array(
      '#type' => 'number',
      '#description' => t("Priority."),
      '#min' => 0,
      '#max' => 1,
      '#step' => 0.1,
      '#states' => array(
        'visible' => array(
          ':input[name="custom_url_sitemap['.$i.'][url]"]' => array('filled' => TRUE),  
        ),
      ),
    );

  // botones
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save custom url settings'),
  );

  } // end for





  // Adds "Add another name" button.
  $form['add_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add another exception'),
    '#submit' => array('add_more_url'),
  );

  // If we have more than one name, this button allows removal of the
  // last name.
  if ($form_state['num_custom_url_sitemap'] > 1) {
    $form['remove_url'] = array(
      '#type' => 'submit',
      '#value' => t('Remove latest name'),
      '#submit' => array('remove_url'),
      // Since we are removing a name, don't validate until later.
      '#limit_validation_errors' => array(),
    );
  }

}


*/


// muestra los nombres de los node types
function query_node_types_name(){
  $types = node_type_get_types();
  $types = array_keys($types);
  return $types;

}



/**
 * Submit handler for "Add another name" button on xml_sitemap_simple_tutorial_9().
 */
function add_more_url($form, &$form_state) {
  // Everything in $form_state is persistent, so we'll just use
  // $form_state['add_name']
  $form_state['num_custom_url_sitemap']++;

  // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
  $form_state['rebuild'] = TRUE;
}



/**
 * Submit handler for "Remove name" button on xml_sitemap_simple_tutorial_9().
 */
function remove_url($form, &$form_state) {
  if ($form_state['num_custom_url_sitemap'] > 1) {
    $form_state['num_custom_url_sitemap']--;
  }

  // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
  $form_state['rebuild'] = TRUE;
}



/**
 * 
 */
function create_checkbox_element($title, $valorActiveDefault = 0){

  $value['#type'] = 'checkbox';
  $value['#title'] = $title;
  $value['#default_value'] = $valorActiveDefault;

  return $value;
}

/**
 * 
 */
function create_number_element($activeElement, $valorActiveDefault = 0.5){

  $value['#type'] = 'number';
  $value['#description'] = t('Priority');
  $value['#min'] = 0;
  $value['#max'] = 1;
  $value['#step'] = 0.1;
  $value['#states'] = array(
      'visible' => array(
        ':input[name="'.$activeElement.'"]' => array('checked' => TRUE),
      ),
    );

  $value['#default_value'] = $valorActiveDefault;

  return $value;
}

/**
 * 
 */
function create_frequency_element($activeElement, $valorActiveDefault = 'monthly'){

  $value['#type'] = 'select';
  $value['#description'] = t('Change frequency');
  $value['#options'] = array(
      'never' => t('never'),
      'yearly' => t('yearly'),
      'monthly' => t('monthly'),
      'weekly' => t('weekly'),
      'daily' => t('daily'),
      'hourly' => t('hourly'),
      'always' => t('always'), 
  );
  $value['#states'] = array(
      'visible' => array(
        ':input[name="'.$activeElement.'"]' => array('checked' => TRUE),
      ),
    );

  $value['#default_value'] = $valorActiveDefault;

  return $value;
}


/**
 * 
 * @return array of views pages with path defined
 *
 * this is the same code as base_meta_form_views
 * on base_meta module
 */
function xml_sitemap_simple_get_views_pages() {
  
  $result = array();
  $views = views_get_enabled_views();
  foreach ($views as $view) {

    if (empty($view->display)) {
      // Skip this view as it is broken.
      debug(t("Skipping broken view @view", array('@view' => $view->name)));
      continue;
    }
    
    foreach (array_keys($view->display) as $id) {
      $plugin = views_fetch_plugin_data('display', $view->display[$id]->display_plugin);

      if ($plugin['name'] == 'page') {
        $view_name = $view->name;
        $page_path = $view->display[$id]->display_options['path'];
        // Skip admin views
        if (strpos($page_path, 'admin/') === FALSE) {
          $result[][$view_name] = $page_path;
        }
      }           
    }
  }
  
  return $result;
}

function xml_sitemap_simple_get_views_names() {
  $viewsPages = xml_sitemap_simple_get_views_pages();
  $viewsNames = array();
  foreach ($viewsPages as $viewsPage) {
    foreach ($viewsPage as $key => $value) {
      $viewsNames[] = $key;
    }
  }
  return $viewsNames;
}

