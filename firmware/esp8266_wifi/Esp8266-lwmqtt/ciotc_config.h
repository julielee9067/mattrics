/******************************************************************************
 * Copyright 2018 Google
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
// This file contains your configuration used to connect to Cloud IoT Core

// WIFI
// Wifi network details.
const char *ssid = "Cloudwifi-167-504-P"; // "mattrics";//
const char *password = "CWAE1923"; // "mattrics123";//

// Cloud iot details.
const char *project_id = "mattrics-338705";
const char *location = "us-central1";
const char *registry_id = "ESP01S";
const char *device_id = "sql-board";

// Configuration for NTP
const char* ntp_primary = "time.google.com";
const char* ntp_secondary = "pool.ntp.org";

// To get the private key run (where private-key.pem is the ec private key
// used to create the certificate uploaded to google cloud iot):
// openssl ec -in <private-key.pem> -noout -text
// and copy priv: part.
// The key length should be exactly the same as the key length bellow (32 pairs
// of hex digits). If it's bigger and it starts with "00:" delete the "00:". If
// it's smaller add "00:" to the start. If it's too big or too small something
// is probably wrong with your key.
const unsigned char private_key[] = {
//  0x5a, 0x2e, 0x06, 0xb5, 0xc1, 0xf2, 0x9c, 0xb3, 0x77, 0xb2, 0x89, 0xf5, 0x29, 0x29, 0x93,
//  0x07, 0xfd, 0xed, 0x22, 0x0d, 0x03, 0x2b, 0xa6, 0xb1, 0xb6, 0x04, 0x0b, 0xd5, 0x9b, 0x49,
//  0x7d, 0xca

  0x8c, 0xf3, 0x5e, 0x6b, 0x8b, 0x57, 0x6a, 0x1b, 0xc4, 0x8d, 0x0e, 0x4c, 0x79, 0x65, 
  0x0a, 0x70, 0x1d, 0xd8, 0x3a, 0xf8, 0x67, 0xba, 0x64, 0xc0, 0x5d, 0xfe, 0xe0, 0xbe, 0x18,
  0xf3, 0x31, 0xc3};

// Time (seconds) to expire token += 20 minutes for drift
const int jwt_exp_secs = 3600; // Maximum 24H (3600*24)

// Certificates for SSL on the LTS server
const char* primary_ca = "-----BEGIN CERTIFICATE-----\n"
    "MIIBxTCCAWugAwIBAgINAfD3nVndblD3QnNxUDAKBggqhkjOPQQDAjBEMQswCQYD\n"
    "VQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzERMA8G\n"
    "A1UEAxMIR1RTIExUU1IwHhcNMTgxMTAxMDAwMDQyWhcNNDIxMTAxMDAwMDQyWjBE\n"
    "MQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExM\n"
    "QzERMA8GA1UEAxMIR1RTIExUU1IwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAATN\n"
    "8YyO2u+yCQoZdwAkUNv5c3dokfULfrA6QJgFV2XMuENtQZIG5HUOS6jFn8f0ySlV\n"
    "eORCxqFyjDJyRn86d+Iko0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUw\n"
    "AwEB/zAdBgNVHQ4EFgQUPv7/zFLrvzQ+PfNA0OQlsV+4u1IwCgYIKoZIzj0EAwID\n"
    "SAAwRQIhAPKuf/VtBHqGw3TUwUIq7TfaExp3bH7bjCBmVXJupT9FAiBr0SmCtsuk\n"
    "miGgpajjf/gFigGM34F9021bCWs1MbL0SA==\n"
    "-----END CERTIFICATE-----\n";

//const char* primary_ca = "    -----BEGIN CERTIFICATE-----
//MIIDaDCCAlCgAwIBAgIEVLbWLjANBgkqhkiG9w0BAQsFADCBgDEtMCsGA1UELhMk
//MTIzNTA2OWItZDdmMy00YWU1LWFhMWMtMGIzOWRmY2E3ZGE1MSwwKgYDVQQDEyNH
//b29nbGUgQ2xvdWQgU1FMIENsaWVudCBDQSBtYXR0cmljczEUMBIGA1UEChMLR29v
//Z2xlLCBJbmMxCzAJBgNVBAYTAlVTMB4XDTIyMDExOTA1MzUyMFoXDTMyMDExNzA1
//MzYyMFowNjERMA8GA1UEAxMIbWF0dHJpY3MxFDASBgNVBAoTC0dvb2dsZSwgSW5j
//MQswCQYDVQQGEwJVUzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAJat
//isFHa6BWPUwGHoA3dHCkqCy3lWloc25ehbPtbMCfiup9xWMobBHs946kKSEEmfm9
//4EXNFV6WVl2W81gHAKY3mqHsnm504fOQU/n8yQkRMMKfi8AB/TIOlQHmacR1K0n8
//MOCqs2kosFNNVZ134Z7ZwC92cFH0aLMbp0Cx6WZuCbf+UOhq/SJYB3aSnSOWKOdc
//M7ciANYL6k9XNjdrLPZlzGxC+uNQWeIFu1OdJ46qYQt8tfx4FPJPZ0ZZinoHD9OG
//0pbaKuT6G/uSWweeWtWO7ro5QuWXYYe7HcgOBbQAbpLzeW9xatxJjGpHFALvAYIg
//e+9+6OGQ4voqzQ2FL3kCAwEAAaMzMDEwCQYDVR0TBAIwADAkBgNVHREEHTAbgRlk
//bXN3bGRtc3dsMTIzMjVAZ21haWwuY29tMA0GCSqGSIb3DQEBCwUAA4IBAQA0ds7L
//CJHetLSYEkhJ4bDzommKgKMVmiU8zSi4FyZgy6XkFjSOVL+izVGyE5vGtkr3Ui5s
//73iyvQwH2xbORr9oFpU3FPPkvJig5KU32wuqUj63mU9pZ+hK+LgcN36TXFVRPXD9
//UafNGcfaAmrvI/rF2plHWANOhgQREkeC8tjqXyETPNnW5fgiRP/mnqIbYZZ9R7+7
///kiW/yk5skC3tVQLKnS//uyMk2fQcdghucbos8nomSxKowopYXKhvX2eTQ0GXetf
//7mkZqSeLn/otyidxFF+mf7X7ZptGgbWHPI/8FidUBnseZOqyooqiPERgFWRudg2y
//pTA4fdRUiyQTO7C7
//-----END CERTIFICATE-----\n"

const char* backup_ca = "-----BEGIN CERTIFICATE-----\n"
    "MIIB4TCCAYegAwIBAgIRKjikHJYKBN5CsiilC+g0mAIwCgYIKoZIzj0EAwIwUDEk\n"
    "MCIGA1UECxMbR2xvYmFsU2lnbiBFQ0MgUm9vdCBDQSAtIFI0MRMwEQYDVQQKEwpH\n"
    "bG9iYWxTaWduMRMwEQYDVQQDEwpHbG9iYWxTaWduMB4XDTEyMTExMzAwMDAwMFoX\n"
    "DTM4MDExOTAzMTQwN1owUDEkMCIGA1UECxMbR2xvYmFsU2lnbiBFQ0MgUm9vdCBD\n"
    "QSAtIFI0MRMwEQYDVQQKEwpHbG9iYWxTaWduMRMwEQYDVQQDEwpHbG9iYWxTaWdu\n"
    "MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEuMZ5049sJQ6fLjkZHAOkrprlOQcJ\n"
    "FspjsbmG+IpXwVfOQvpzofdlQv8ewQCybnMO/8ch5RikqtlxP6jUuc6MHaNCMEAw\n"
    "DgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFFSwe61F\n"
    "uOJAf/sKbvu+M8k8o4TVMAoGCCqGSM49BAMCA0gAMEUCIQDckqGgE6bPA7DmxCGX\n"
    "kPoUVy0D7O48027KqGx2vKLeuwIgJ6iFJzWbVsaj8kfSt24bAgAXqmemFZHe+pTs\n"
    "ewv4n4Q=\n"
    "-----END CERTIFICATE-----\n";

// In case we ever need extra topics
const int ex_num_topics = 0;
const char* ex_topics[ex_num_topics];
//const int ex_num_topics = 1;
//const char* ex_topics[ex_num_topics] = {
//  "/devices/my-device/tbd/#"
//};
