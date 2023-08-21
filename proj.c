/* Make sure you have these dependencies on your system */
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <jansson.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

/* This function gets executed as soon as the API request is performed and the response is returned */
void function_pt(void *ptr, size_t size, size_t nmemb, void *stream)
{ // ignore the last three arguments, aren't used in our case
  printf("\n\n%s\n\n", ptr); // prints the response on screen
  // printf("%d", atoi(ptr));

  /* The five lines below parse the data returned from the response; if you want to read more on what do the functions do,
  you can read the content given here: https://jansson.readthedocs.io/en/2.8/apiref.html */
  json_t *js_obj      = json_loads(ptr, 0, NULL);
  json_t *js_name  = json_object_get(js_obj, "name");
  json_t *js_main  = json_object_get(js_obj, "main");
  json_t *js_temp    = json_object_get(js_main, "temp");
  json_t *js_humidity = json_object_get(js_main, "humidity");

  /* Displaying some parsed values if they follow the right format, again, please see the docs of library 
  to see the usage of functions */
  printf("City    = %s\n", json_is_string(js_name) ?
          json_string_value(js_name) : "unknown");
  char snum[5];

  printf("Temperature = %.2f Deg Celc.\n", json_is_number(js_temp) ?
          json_number_value(js_temp) : 0);
  printf("Humidity = %.2f Percent\n\n", json_is_number(js_humidity) ?
          json_number_value(js_humidity) : 0);
}

int main(void)
{
  /* Performing an HTTP request using cURL library */

  /* Here we are creating a CURL easy handle and initializing it using the function curl_easy_init() and storing it using a pointer “curl”. 
  The curl_easy_init() must be the first function to call when using with libcurl and it returns a CURL easy handle that you must use as input to other functions in the easy interface, which we store in the previously created. */
  CURL *curl; 
  curl = curl_easy_init();


  if(curl) { // We are checking whether or not the CURL easy handle was successfully created

    /* It defines the behavior of libcurl. It takes three arguments :
    curl : The Curl easy handle
    CURLOPT_URL : Specify that we’ll be pass in the URL to work with
    http://example.com : The URL we will request */
    curl_easy_setopt(curl, CURLOPT_URL, "api.openweathermap.org/data/2.5/weather?q=Fairfax&units=metric&appid=c1b8806bdf9a75f4841a04169649f404"); // replace 'Fairfax' with the name of your city and appid with your key
    
    /* Here we link the user defined function 'function_pt' with the handle, so as soon as the response is received, that function is called */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, function_pt);

    /* Perform the request */
    curl_easy_perform(curl);

    /* The curl_easy_cleanup() should always be the last function to call when working with Curl easy sessions. It kills the handle passed to it and all memory associated with it. Then we simply return an integer and exit the program. */
    curl_easy_cleanup(curl);
  }

  system("read -p 'Press Enter to continue...' var"); // wait for the user key press
  return 0;
}