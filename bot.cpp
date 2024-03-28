#include <iostream>
#include <curl/curl.h>
#include <sstream>

int main() {
  // Initialize the curl library
  curl_global_init(CURL_GLOBAL_DEFAULT);

  // Create a curl handle
  CURL *handle = curl_easy_init();

  // Set the URL to request
  curl_easy_setopt(handle, CURLOPT_URL, "https://www.example.com");

  // Set the request method to POST
  curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");

  // Set the request body
  curl_easy_setopt(handle, CURLOPT_POSTFIELDS, "hello world");

  // Set the callback function to handle the response
//   curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, [](char *data, size_t size, size_t nmemb, void *userp) {
    // Cast the user pointer to an ostream and write the data to it
    // *static_cast<std::ostream*>(userp) << data;

    // Return the number of bytes processed
    // return size * nmemb;
//   }
//   );

  // Set the user pointer to be an ostream to which the response will be written
  std::ostringstream response;
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);

  // Perform the request
  CURLcode result = curl_easy_perform(handle);

  // Check the result
  if (result != CURLE_OK) {
    // If the request failed, print an error message
    std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(result) << std::endl;
  } else {
    // If the request was successful, print the response
    std::cout << response.str() << std::endl;
  }

  // Clean up
  curl_easy_cleanup(handle);
  curl_global_cleanup();

  return 0;
}