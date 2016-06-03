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

  create_sitemap($sitemap_data);

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




