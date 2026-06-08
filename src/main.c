#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *data;
  size_t size;
} Response;

static size_t write_cb(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  Response *res = userp;
  res->data = realloc(res->data, res->size + realsize + 1);
  memcpy(&res->data[res->size], contents, realsize);
  res->size += realsize;
  res->data[res->size] = '\0';
  return realsize;
}

Response fetch(const char *url) {
  Response res = {.data = malloc(1), .size = 0};
  CURL *curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  return res;
}

void list_currencies(void) {
  Response r = fetch("https://api.frankfurter.app/currencies");
  cJSON *json = cJSON_Parse(r.data);
  free(r.data);

  cJSON *item = NULL;
  cJSON_ArrayForEach(item, json) {
    printf("%-5s %s\n", item->string, item->valuestring);
  }
  cJSON_Delete(json);
}

void exchange(double amount, const char *from, const char *to) {
  char url[256];
  snprintf(url, sizeof(url),
           "https://api.frankfurter.app/latest?amount=%.2f&from=%s&to=%s",
           amount, from, to);

  Response r = fetch(url);
  cJSON *json = cJSON_Parse(r.data);
  free(r.data);

  cJSON *rates = cJSON_GetObjectItem(json, "rates");
  cJSON *result = cJSON_GetObjectItem(rates, to);

  if (result) {
    printf("%.2f %s = %.2f %s\n", amount, from, result->valuedouble, to);
  } else {
    fprintf(stderr, "Invalid currency: %s\n", to);
  }

  cJSON_Delete(json);
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    list_currencies();
  } else if (argc == 3) {
    exchange(1.0, argv[1], argv[2]);
  } else if (argc == 4) {
    exchange(atof(argv[1]), argv[2], argv[3]);
  } else {
    fprintf(stderr, "Usage:\n  curx\n  curx USD SEK\n  curx 100 USD SEK\n");
    return 1;
  }
}
