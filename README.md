# xml_sitemap_simple

This module provides a sitemap.xml file

## Backdrop port status

This is not port of Drupal 7 modules.


## Basic options

The module adds options to select:

-   which content type(s) to search,

-   which taxonomy term(s) to search,

-   which specific module search to use (node, help, user or any module that
    implements search),

-   advanced criteria

For all these choices, there are options to switch between a select box,
checkboxes or radio buttons, and also customize the selector label and the
default - Any - text.

## Advanced options

There are also options to:

change the default search box label, add a default text in the search box, add
advanced search criteria, change the default submit button text, use an image
instead of the submit button, change the order of all the elements, include some
elements in a popup block, add a filter to the results page, show/hide basic
and/or advanced search in the results page, show/hide meta data in the results
page, multiple search paths…

## Included submodules

-   Custom Search Taxonomy: taxonomy options for the search block

-   Custom Search Internationalization: search content from all or current
    language only, and all label and selectors translation handling

## Installation

-   Install this module using the official Backdrop CMS instructions at
    https://backdropcms.org/guide/modules.

-   Go to Structure \> Layouts (admin/structure/layouts) to add
    Custom Search Blocks as you would do for any other block.
    All the settings are available in the block config.

-   Go to Configuration \> Search and metadata \> Custom search 
    (admin/config/search/custom\_search/settings) to change settings
    for the results page.

## License

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

## Current Maintainers

-   David Vinagre (https://github.com/davidvina)

## Credits

This module was originally written for Backdrop CMS by David Vinagre
(https://github.com/davidvina).
