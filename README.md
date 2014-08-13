Hosting Varnish
===============

Varnish for Aegir


Installation
============

Instructions are in progress!

1. Install and configure Varnish3.

  ```
  $ sudo apt-get install varnish
  ```

  Create relevant VCLs (@TODO: elaborate?)

2. Check that Varnish is properly configured.
  Create test sites within the Aegir front-end. 

  Example URLs:
    - d6.varnish4.localhost
    - d7.varnish4.localhost
  
3. Test that Apache connection is successful:
  ```
  $ curl -I d7.varnish4.localhost:8080
  HTTP/1.1 200 OK
  Date: Thu, 07 Aug 2014 19:02:22 GMT
  Server: Apache/2.2.22 (Debian)
  X-Powered-By: PHP/5.4.4-14+deb7u12
  Expires: Sun, 19 Nov 1978 05:00:00 GMT
  Last-Modified: Thu, 07 Aug 2014 19:02:22 +0000
  Cache-Control: no-cache, must-revalidate, post-check=0, pre-check=0
  ETag: "1407438142"
  Content-Language: en
  X-Generator: Drupal 7 (http://drupal.org)
  Vary: Accept-Encoding
  Content-Type: text/html; charset=utf-8
  ```

3. Test the expected Varnish MISS:
  ```
  $ curl -I d7.varnish4.localhost:9999

  HTTP/1.1 200 OK
  Server: Apache/2.2.22 (Debian)
  X-Powered-By: PHP/5.4.4-14+deb7u12
  Expires: Sun, 19 Nov 1978 05:00:00 GMT
  Last-Modified: Thu, 07 Aug 2014 19:08:57 +0000
  Cache-Control: no-cache, must-revalidate, post-check=0, pre-check=0
  ETag: "1407438537"
  Content-Language: en
  X-Generator: Drupal 7 (http://drupal.org)
  Vary: Accept-Encoding
  Content-Type: text/html; charset=utf-8
  Date: Thu, 07 Aug 2014 19:08:58 GMT
  X-Varnish: 1490586328
  Age: 0
  Via: 1.1 varnish
  Connection: keep-alive
  X-Varnish-Cache: MISS
  ```

4. Update cache settings (on the hosted sites) to allow anonymous caching:

  http://d7.varnish4.localhost:8080/admin/config/development/performance

  You only need to check Cache pages for anonymous users, and set a max cache time ("Expiration of cached pages").
  
  *@TODO: I believe Guillaume automated this step? https://github.com/jonpugh/hosting_varnish/blob/6.x-1.x/hosting_varnish.drush.inc#L14*

5. Verify that the cache gets HIT (on second try):
  
  ```
  $ curl -I d7.varnish4.localhost:9999

  HTTP/1.1 200 OK
  Server: Apache/2.2.22 (Debian)
  X-Powered-By: PHP/5.4.4-14+deb7u12
  X-Drupal-Cache: MISS
  Expires: Sun, 19 Nov 1978 05:00:00 GMT
  Last-Modified: Thu, 07 Aug 2014 19:18:34 +0000
  Cache-Control: public, max-age=60
  ETag: "1407439114-1"
  Content-Language: en
  X-Generator: Drupal 7 (http://drupal.org)
  Vary: Cookie,Accept-Encoding
  Content-Type: text/html; charset=utf-8
  Date: Thu, 07 Aug 2014 19:18:39 GMT
  X-Varnish: 1490586342 1490586339
  Age: 5
  Via: 1.1 varnish
  Connection: keep-alive
  X-Varnish-Cache: HIT
  ```

6. Auto-configuring your sites to be cached through Varnish.

  Create a new site in the aegir front-end to make sure you have an unconfigured control.

  Install and configure hosting_varnish and varnish module. (The default values should work, for just auto-caching.)

  Test that the pre-hosting_varnish site still isn't cached:

  ```
  $ # Do this twice to be 100% sure.
  $ curl -I test6.local:9999
  
  HTTP/1.1 200 OK
  Server: Apache/2.2.22 (Debian)
  X-Powered-By: PHP/5.4.4-14+deb7u12
  Expires: Sun, 19 Nov 1978 05:00:00 GMT
  Last-Modified: Thu, 07 Aug 2014 21:14:07 +0000
  Cache-Control: no-cache, must-revalidate, post-check=0, pre-check=0
  ETag: "1407446047"
  Content-Language: en
  X-Generator: Drupal 7 (http://drupal.org)
  Vary: Accept-Encoding
  Content-Type: text/html; charset=utf-8
  Accept-Ranges: bytes
  Date: Thu, 07 Aug 2014 21:14:07 GMT
  X-Varnish: 480140253
  Age: 0
  Via: 1.1 varnish
  Connection: keep-alive
  X-Varnish-Cache: MISS
  ```

7. Create a site.

  Test that the post-hosting_varnish new site still isn't cached:

  ```
  $ # You need to do this command twice as the first MISS is normal.
  $ curl -I test7.local:9999

  HTTP/1.1 200 OK
  Server: Apache/2.2.22 (Debian)
  X-Powered-By: PHP/5.4.4-14+deb7u12
  X-Drupal-Cache: MISS
  Expires: Sun, 19 Nov 1978 05:00:00 GMT
  Last-Modified: Thu, 07 Aug 2014 21:14:11 +0000
  Cache-Control: public, max-age=60
  ETag: "1407446051-1"
  Content-Language: en
  X-Generator: Drupal 7 (http://drupal.org)
  Vary: Cookie,Accept-Encoding
  Content-Type: text/html; charset=utf-8
  Date: Thu, 07 Aug 2014 21:14:12 GMT
  X-Varnish: 480140256 480140255
  Age: 1
  Via: 1.1 varnish
  Connection: keep-alive
  X-Varnish-Cache: HIT
  ```
