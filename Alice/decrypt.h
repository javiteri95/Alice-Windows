#include <civetweb.h>
#include "cJSON.h"
#include "cors.h"
#include <string>
#include <iostream>
#include <vector>
#include "helpers.h"
#include <time.h>
#include "signal.h"
#include "spdlog/spdlog.h"

int postDecryptEmail(struct mg_connection *conn, void *cbdata, char *dbPath, char* password);
int postDecryptKey(struct mg_connection *conn, void *cbdata, char *dbPath);