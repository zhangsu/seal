#include <string.h>
#include <stdio.h>
#include <seal/threading.h>
#include <seal.h>
#include "test.h"

const char minimal_wav[] =
"\x52\x49\x46\x46\x40\x09\x00\x00\x57\x41\x56\x45\x66\x6D\x74\x20\x10\x00\x00"
"\x00\x01\x00\x01\x00\x11\x2B\x00\x00\x11\x2B\x00\x00\x01\x00\x08\x00\x64\x61"
"\x74\x61\xCA\x08\x00\x00\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x7E"
"\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x80\x7E\x7E\x80\x80\x80\x80\x80\x80\x80\x7E\x7E"
"\x7E\x7E\x7E\x7E\x7E\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
"\x7E\x7E\x7E\x7E\x7E\x7E\x7E\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80"
"\x80\x80\x80\x7E\x7E\x7E\x7D\x7D\x7E\x7E\x80\x82\x82\x82\x82\x82\x82\x82\x82"
"\x82\x82\x82\x82\x82\x82\x80\x7E\x7D\x7B\x79\x7B\x80\x82\x82\x82\x82\x82\x84"
"\x84\x84\x84\x84\x84\x84\x84\x84\x84\x84\x85\x84\x7B\x71\x7B\x85\x84\x85\x85"
"\x85\x85\x85\x85\x85\x85\x87\x87\x87\x87\x89\x84\x85\x87\x8A\x80\x67\x85\x89"
"\x89\x8A\x89\x8A\x89\x8A\x8A\x8E\x8A\x90\x7E\x4D\x82\x90\x89\x90\x87\x6E\x64"
"\x77\x91\x8A\x8E\x8B\x8E\x8E\x8E\x8E\x91\x8B\x97\x43\x84\x67\x3B\x93\x90\x7B"
"\x62\x62\x6E\x91\x89\x8E\x8B\x8E\x8E\x8E\x8E\x93\x8A\x9D\x4A\x77\x7B\x2F\x8E"
"\x91\x7E\x5F\x60\x67\x90\x8A\x8E\x8B\x8B\x8E\x8E\x8B\x93\x89\x9F\x55\x66\x87"
"\x2F\x87\x93\x82\x5D\x66\x66\x8B\x8B\x8B\x8E\x8B\x8E\x90\x8B\x93\x89\x9D\x62"
"\x59\x90\x2E\x7B\x95\x85\x60\x6C\x64\x89\x8E\x8A\x8E\x8B\x8E\x90\x8B\x93\x8A"
"\x9B\x6F\x4D\x95\x33\x6F\x97\x89\x67\x62\x5F\x85\x90\x8A\x8E\x8B\x8E\x90\x8B"
"\x93\x8B\x99\x7D\x46\x99\x3B\x60\x99\x8A\x6A\x5F\x5D\x7E\x90\x89\x8E\x8B\x8E"
"\x8E\x8B\x91\x8B\x93\x87\x42\x95\x48\x55\x99\x8B\x6E\x60\x62\x79\x91\x89\x8E"
"\x8B\x8E\x8E\x8E\x90\x90\x90\x91\x3E\x8E\x59\x43\x97\x8E\x71\x64\x67\x73\x93"
"\x89\x8E\x8B\x8E\x8E\x8E\x90\x91\x8B\x97\x42\x84\x67\x3B\x93\x90\x79\x60\x62"
"\x6F\x91\x89\x8E\x8B\x8E\x8E\x8E\x8E\x93\x8A\x9D\x4B\x77\x79\x31\x8E\x91\x7D"
"\x5A\x5D\x66\x90\x8A\x8E\x8B\x8B\x8E\x8E\x8B\x93\x89\x9F\x57\x6C\x85\x2F\x87"
"\x93\x82\x5F\x66\x64\x8B\x8B\x8B\x8E\x8B\x8E\x90\x8B\x93\x89\x9D\x60\x5D\x90"
"\x2E\x7B\x95\x85\x62\x6C\x64\x89\x8E\x8B\x8E\x8B\x8E\x90\x8B\x93\x8A\x9B\x6E"
"\x4D\x97\x33\x6F\x97\x89\x67\x62\x62\x85\x8E\x8A\x8E\x8B\x8E\x90\x8B\x93\x8B"
"\x99\x7D\x43\x99\x3C\x60\x99\x8A\x66\x5D\x5F\x7E\x90\x89\x8E\x8B\x8E\x8E\x8B"
"\x91\x8E\x93\x89\x42\x95\x48\x55\x99\x8B\x6C\x5F\x62\x79\x91\x89\x8E\x8B\x8E"
"\x8E\x8B\x90\x90\x90\x90\x40\x8E\x57\x46\x97\x8E\x71\x64\x64\x71\x93\x89\x8E"
"\x8B\x8E\x8E\x8E\x90\x90\x8E\x95\x40\x87\x5F\x43\x97\x8B\x7E\x5D\x5F\x6E\x91"
"\x89\x8E\x8B\x8B\x8E\x8E\x8E\x91\x8A\x9B\x43\x7B\x6F\x39\x93\x8B\x84\x5F\x60"
"\x67\x90\x8A\x8B\x8B\x8B\x8E\x8E\x8B\x93\x89\x9D\x52\x6C\x7E\x31\x8B\x8E\x87"
"\x64\x6A\x66\x8B\x8B\x8B\x8E\x8B\x8E\x8E\x8B\x93\x89\x9D\x5F\x60\x89\x2F\x85"
"\x91\x89\x64\x64\x64\x89\x8E\x8A\x8E\x8B\x8E\x90\x8B\x93\x89\x9D\x6A\x53\x90"
"\x31\x7D\x93\x8B\x67\x5A\x5F\x85\x8E\x8A\x8E\x8B\x8E\x8E\x8B\x93\x8A\x99\x76"
"\x48\x95\x36\x6F\x95\x8B\x6E\x5F\x60\x7E\x90\x89\x8E\x8B\x8B\x8E\x8B\x91\x8B"
"\x93\x82\x40\x95\x40\x60\x97\x8B\x73\x66\x66\x79\x91\x89\x8E\x8B\x8B\x8E\x8B"
"\x90\x8E\x90\x8E\x40\x91\x4F\x52\x99\x8B\x79\x5F\x62\x76\x91\x89\x8E\x8B\x8B"
"\x8E\x8E\x90\x90\x8B\x95\x40\x87\x5D\x43\x97\x8B\x7E\x5A\x5F\x6F\x90\x89\x8E"
"\x8B\x8B\x8E\x8E\x8E\x91\x8A\x9B\x48\x7D\x6E\x3B\x93\x8E\x84\x5D\x60\x67\x8E"
"\x8A\x8B\x8B\x8B\x8B\x8E\x8B\x91\x89\x9D\x4F\x6F\x7B\x33\x8E\x8E\x85\x64\x6A"
"\x64\x8B\x8A\x8B\x8B\x8B\x8B\x8E\x8B\x93\x87\x9F\x59\x66\x82\x2F\x8E\x8B\x8E"
"\x66\x5D\x64\x89\x8B\x8A\x8E\x8B\x8B\x8E\x8B\x93\x87\x9D\x66\x57\x8B\x2F\x84"
"\x90\x8E\x6E\x5A\x60\x85\x8E\x8A\x8E\x8B\x8B\x8E\x8A\x93\x89\x99\x73\x4B\x90"
"\x31\x79\x91\x8B\x71\x66\x64\x7E\x90\x89\x8E\x8B\x8B\x8E\x8B\x91\x8A\x95\x80"
"\x43\x91\x39\x6C\x95\x8E\x76\x5F\x64\x77\x91\x89\x8E\x8B\x8B\x8E\x8B\x91\x8B"
"\x91\x89\x40\x91\x43\x5F\x97\x8B\x7E\x5A\x5F\x76\x91\x89\x8E\x8B\x8B\x8E\x8B"
"\x8E\x90\x8B\x95\x40\x85\x57\x48\x99\x8A\x84\x62\x66\x6C\x90\x89\x8B\x8B\x8B"
"\x8B\x8E\x8B\x93\x89\x9D\x4D\x77\x71\x36\x95\x8A\x8B\x62\x5F\x67\x8B\x8A\x8B"
"\x8B\x8B\x8E\x8E\x8B\x93\x89\x9F\x5D\x62\x82\x2F\x8A\x8B\x8B\x6A\x67\x66\x87"
"\x8E\x8A\x8E\x8B\x8E\x90\x8B\x93\x89\x9B\x71\x53\x91\x2F\x7D\x91\x90\x71\x59"
"\x60\x82\x90\x89\x8E\x8B\x8E\x90\x8E\x93\x8B\x97\x84\x43\x93\x3E\x67\x95\x8B"
"\x79\x62\x66\x77\x93\x8A\x90\x8E\x90\x90\x90\x91\x90\x90\x90\x3E\x8B\x4F\x4F"
"\x9D\x8B\x89\x64\x5F\x73\x95\x8B\x90\x8E\x8B\x8A\x8A\x8E\x95\x8E\x9F\x4A\x84"
"\x66\x4B\x9F\x8B\x93\x6A\x64\x67\x87\x89\x91\x91\x91\x93\x93\x93\x99\x8E\xA7"
"\x59\x77\x7E\x42\x93\x80\x97\x73\x6A\x6A\x93\x97\x95\x97\x97\x97\x9B\x97\xA0"
"\x93\x93\x6C\x66\x93\x40\x97\x9B\xA0\x80\x71\x5A\x5F\xA0\x82\x7D\x89\x8E\xA2"
"\x99\xA7\x97\xAE\x84\x67\x85\x26\x79\x55\x79\x53\x4D\x62\x8A\x9F\x99\x9D\x9D"
"\x99\xA7\x71\x84\x64\x8E\xA0\x2B\x6F\x42\x76\xA5\x9B\x93\x6F\x6E\x8A\x77\x7B"
"\x62\x89\xA7\x76\x71\x93\xA0\x9B\xA5\x53\x95\x66\x67\x7E\x7E\x5F\x62\x79\x4F"
"\x71\x8B\x9F\x99\x9D\x9D\x9F\x9B\xAA\x6C\x90\x1E\x73\x85\x1F\x7E\x8A\xA0\x71"
"\x6A\x73\x9D\x99\xA2\x76\x82\x60\x8E\xA5\x7D\x67\xA2\x6E\x6E\x90\x42\xA0\x99"
"\xA7\x52\x59\x31\x87\xA5\x6F\x71\x8E\x9F\x9B\x9B\xA2\x99\xAA\x87\x33\x82\x01"
"\x82\xA7\x79\x5A\x62\x6F\x8B\x9F\x99\x9F\x9B\xA0\x71\x82\x64\x90\xA8\x6C\x26"
"\x91\x4D\x79\xA5\x9D\x90\x71\x7B\x59\x85\x5A\x93\xA0\x73\x77\x95\x9F\x9F\x9D"
"\xA5\x53\x99\x64\x34\x8E\x5A\x90\x71\x40\x4F\x97\x99\x9B\x9B\x9D\x9D\x9F\xA0"
"\x73\x7D\x6E\x52\x8B\x43\x27\xA0\x97\x9B\x71\x77\x79\x9D\x9F\x6F\x82\x5F\x97"
"\xA0\x71\x79\x90\xAF\x66\x79\x89\x42\xA7\x9B\x71\x5A\x27\x6F\x97\x6F\x71\x93"
"\x9D\x9B\x9D\x9D\xA2\x99\xAE\x4B\x4A\x5A\x3B\x97\x73\x77\x7B\x6E\x71\x8B\xA2"
"\x97\xA0\x99\x6E\x84\x5A\xA0\x97\x7B\x60\x4B\xA0\x42\x82\xA5\x9B\x8E\x6E\x3C"
"\x67\x60\x97\x9D\x6E\x76\x97\x9B\x9D\x9B\xA0\x9B\x57\x99\x27\x4F\x67\x99\x91"
"\x40\x42\x77\x9F\x95\x9B\x9B\x99\x9F\x97\x6E\x7E\x59\xA8\x52\x62\x4A\x59\xA7"
"\x95\x97\x79\x6F\x7D\x97\x67\x7D\x5A\x9B\x95\x6C\x6F\x9B\x93\xA8\x62\x82\x82"
"\x55\x9B\x66\x7E\x39\x77\x71\x66\x73\x91\x97\x95\x97\x9B\x93\xA2\x8B\x79\x57"
"\x31\x95\x4B\x6A\x71\x95\x80\x77\x73\x91\x95\x9B\x90\x67\x77\x57\x9B\x90\x64"
"\x77\x80\x6C\x95\x5A\x8B\x95\x9D\x82\x4A\x55\x4B\x9D\x8A\x67\x71\x91\x93\x93"
"\x95\x97\x95\x97\x60\x67\x4A\x40\xA0\x89\x60\x55\x79\x85\x93\x90\x95\x90\x99"
"\x8A\x6A\x6F\x5D\x9D\x8A\x40\x71\x76\x7B\x97\x8E\x95\x7D\x85\x79\x6A\x6C\x5A"
"\x9B\x8A\x66\x6A\x91\x91\x90\x95\x7E\x85\x90\x64\x67\x6C\x57\x8E\x77\x59\x71"
"\x8E\x8E\x90\x8E\x93\x8B\x99\x84\x66\x6F\x57\x8B\x8A\x59\x55\x89\x7D\x87\x87"
"\x89\x8A\x97\x7E\x62\x67\x5A\x9B\x82\x64\x6A\x8E\x8B\x84\x89\x93\x8A\x97\x7D"
"\x62\x64\x59\x99\x84\x5F\x6C\x90\x8B\x8E\x8B\x91\x8A\x99\x7D\x67\x5D\x55\x97"
"\x77\x5A\x71\x8E\x89\x8E\x8B\x91\x89\x99\x77\x66\x62\x64\x9B\x80\x5D\x6A\x90"
"\x8A\x8E\x8B\x91\x89\x97\x73\x67\x5F\x62\x9D\x77\x5D\x77\x8E\x8A\x8E\x8B\x91"
"\x89\x99\x71\x6C\x5A\x67\x9B\x79\x60\x73\x90\x8A\x8E\x8B\x91\x89\x99\x6F\x67"
"\x5D\x6A\x9D\x73\x5D\x79\x90\x8A\x8E\x8B\x91\x89\x99\x6A\x6A\x57\x6F\x9B\x71"
"\x60\x77\x90\x8A\x8E\x8B\x90\x89\x99\x67\x6E\x59\x71\x9B\x76\x5D\x7B\x90\x8A"
"\x8E\x8B\x90\x8A\x97\x67\x6C\x55\x77\x9B\x6C\x62\x80\x8E\x8A\x8E\x8B\x90\x8B"
"\x95\x67\x6F\x53\x7B\x97\x71\x59\x7B\x90\x8A\x8E\x8B\x90\x8B\x95\x66\x6C\x57"
"\x82\x97\x6C\x60\x84\x8E\x8B\x8B\x8E\x8E\x8E\x91\x62\x6C\x53\x85\x93\x71\x5A"
"\x80\x90\x8A\x8E\x8E\x8E\x8E\x91\x66\x6C\x53\x87\x93\x66\x5D\x85\x8E\x8B\x8B"
"\x8E\x8B\x8E\x90\x60\x6C\x55\x8A\x90\x71\x62\x84\x8E\x8A\x8B\x8E\x8B\x90\x8B"
"\x62\x6C\x53\x8E\x90\x66\x5A\x87\x8B\x8B\x8B\x8E\x8B\x91\x8B\x64\x6A\x53\x90"
"\x8B\x6A\x6A\x89\x8B\x8B\x8B\x90\x8B\x93\x87\x60\x67\x57\x91\x8B\x67\x5D\x89"
"\x8B\x8B\x8B\x90\x8A\x93\x87\x64\x6A\x59\x95\x89\x60\x67\x8B\x8B\x8E\x8B\x90"
"\x8A\x95\x82\x62\x64\x59\x95\x89\x66\x62\x8B\x8B\x8E\x8B\x90\x8A\x95\x80\x60"
"\x64\x5F\x97\x85\x5A\x67\x8E\x8A\x8E\x8A\x91\x89\x97\x7E\x64\x62\x5A\x99\x84"
"\x64\x6A\x8E\x8A\x8B\x8B\x91\x89\x97\x7B\x66\x62\x60\x99\x82\x5F\x6A\x90\x8A"
"\x8E\x8A\x91\x89\x97\x79\x64\x60\x64\x9B\x7E\x60\x73\x8E\x8A\x8E\x8B\x91\x89"
"\x97\x76\x6A\x5D\x66\x9B\x80\x5D\x6A\x90\x89\x8E\x8A\x91\x89\x99\x73\x67\x60"
"\x6A\x9B\x79\x5A\x76\x8E\x8A\x8E\x8B\x90\x89\x99\x6F\x6A\x57\x6C\x99\x7E\x60"
"\x6F\x91\x89\x8E\x8B\x90\x8A\x95\x71\x6C\x55\x76\x99\x79\x59\x77\x90\x89\x8E"
"\x8B\x90\x89\x97\x6C\x67\x5A\x79\x97\x7B\x64\x77\x90\x8A\x8E\x8B\x90\x8A\x95"
"\x6A\x6A\x55\x7B\x97\x76\x53\x79\x90\x89\x8B\x8B\x8E\x8A\x95\x6C\x6A\x55\x80"
"\x95\x77\x66\x7E\x8E\x8A\x8B\x8B\x8E\x8B\x93\x64\x67\x57\x82\x93\x76\x55\x7B"
"\x90\x89\x8B\x8B\x8E\x8B\x91\x67\x6A\x53\x84\x93\x6F\x5F\x82\x8B\x8A\x8B\x8B"
"\x8B\x8B\x90\x62\x67\x55\x87\x90\x73\x5D\x80\x8E\x89\x8B\x8B\x8B\x8B\x8E\x62"
"\x66\x55\x89\x90\x6A\x59\x84\x8A\x89\x8A\x8B\x8A\x8B\x8B\x64\x6A\x59\x8A\x8B"
"\x71\x66\x84\x8A\x89\x8A\x8B\x89\x8E\x89\x64\x67\x5A\x8B\x8B\x6A\x5A\x85\x89"
"\x89\x89\x8A\x89\x8B\x87\x62\x67\x5F\x8B\x89\x6E\x6A\x87\x89\x89\x89\x8A\x87"
"\x8B\x84\x66\x67\x60\x8B\x87\x6E\x60\x87\x87\x87\x87\x89\x85\x8B\x82\x67\x6A"
"\x64\x8B\x85\x6A\x6A\x87\x85\x87\x85\x89\x85\x8B\x80\x67\x6A\x66\x8B\x82\x6E"
"\x6C\x85\x85\x85\x85\x87\x84\x8A\x7D\x6E\x6C\x6A\x8B\x82\x67\x6C\x85\x84\x85"
"\x84\x87\x82\x89\x7D\x6C\x6E\x6F\x8A\x80\x73\x76\x85\x84\x84\x84\x85\x82\x89"
"\x7B\x71\x6E\x71\x89\x80\x6C\x6F\x84\x82\x84\x82\x84\x82\x87\x7B\x76\x6F\x76"
"\x87\x7E\x76\x79\x84\x82\x82\x82\x84\x80\x85\x7B\x77\x76\x76\x85\x7D\x76\x79"
"\x82\x80\x82\x80\x82\x80\x84\x7D\x77\x79\x76\x84\x7B\x79\x7B\x82\x80\x82\x80"
"\x82\x80\x82\x7E\x79\x7E\x71\x77\x77\x7B\x7D\x80\x80\x80\x80\x80\x80\x80\x7E"
"\x79\x80\x7D\x7D\x7E\x7B\x7E\x80\x80\x80\x7E\x80\x80\x80\x80\x7B\x7D\x80\x7E"
"\x7D\x7D\x7E\x80\x7E\x80\x80\x80\x80\x7E\x80\x7E\x7D\x7D\x7D\x7E\x7E\x7E\x7E"
"\x7E\x7E\x7E\x7E\x80\x7E\x7E\x7E\x7E\x7E\x83\x83\x83\x83\x6C\x69\x73\x74\x4A"
"\x00\x00\x00\x49\x4E\x46\x4F\x49\x53\x46\x54\x3E\x00\x00\x00\x46\x69\x6C\x65"
"\x20\x63\x72\x65\x61\x74\x65\x64\x20\x62\x79\x20\x47\x6F\x6C\x64\x57\x61\x76"
"\x65\x2E\x20\x20\x47\x6F\x6C\x64\x57\x61\x76\x65\x20\x63\x6F\x70\x79\x72\x69"
"\x67\x68\x74\x20\x28\x43\x29\x20\x43\x68\x72\x69\x73\x20\x43\x72\x61\x69\x67"
"\x00";

static void
generate_test_file(void)
{
    FILE* audio = fopen(TEST_FILENAME, "wb");
    fwrite(minimal_wav, 1, sizeof (minimal_wav), audio);
    fclose(audio);
}

static void
assert_alloc_src_ok(seal_src_t** src)
{
    *src = seal_alloc_src();
    ASSERT_OK(*src != 0);
    ASSERT_OK(seal_get_src_buf(*src) == 0);
    ASSERT_OK(seal_get_src_type(*src) == SEAL_UNDETERMINED);
    ASSERT_OK(seal_get_src_stream(*src) == 0);
}

static void
assert_new_buf_ok(seal_buf_t** buf)
{
    *buf = seal_new_buf(TEST_FILENAME, SEAL_WAV_FMT);
    ASSERT_OK(*buf != 0);
}

static void
assert_open_stream_ok(seal_stream_t** stream)
{
    *stream = seal_open_stream(TEST_FILENAME, SEAL_WAV_FMT);
    ASSERT_OK(*stream != 0);
}

static void
assert_init_ok(seal_src_t** src, seal_buf_t** buf, seal_stream_t** stream)
{
    assert_alloc_src_ok(src);
    assert_new_buf_ok(buf);
    assert_open_stream_ok(stream);
}

void
test_src_buf(void)
{
    seal_src_t* src;
    seal_buf_t* buf;
    seal_stream_t* stream;

    if (setjmp(next_test)) goto cleanup;

    generate_test_file();
    ASSERT_OK(seal_startup(0));
    assert_init_ok(&src, &buf, &stream);

    ASSERT_OK(seal_set_src_buf(src, buf));
    ASSERT_OK(seal_get_src_type(src) == SEAL_STATIC);

    ASSERT_FAIL(!seal_load2buf(buf, TEST_FILENAME, SEAL_WAV_FMT),
                SEAL_BUF_INUSE);
    ASSERT_FAIL(!seal_free_buf(buf), SEAL_BUF_INUSE);
    ASSERT_FAIL(!seal_set_src_stream(src, stream), SEAL_MIXING_SRC_TYPE);

    ASSERT_OK(seal_play_src(src));
    ASSERT_FAIL(!seal_set_src_buf(src, buf), SEAL_BAD_SRC_OP);
    ASSERT_FAIL(!seal_load2buf(buf, TEST_FILENAME, SEAL_WAV_FMT),
                SEAL_BUF_INUSE);
    ASSERT_FAIL(!seal_free_buf(buf), SEAL_BUF_INUSE);
    ASSERT_FAIL(!seal_set_src_stream(src, stream), SEAL_MIXING_SRC_TYPE);

    seal_pause_src(src);
    ASSERT_NO_ERR();
    ASSERT_FAIL(!seal_set_src_buf(src, buf), SEAL_BAD_SRC_OP);
    ASSERT_FAIL(!seal_load2buf(buf, TEST_FILENAME, SEAL_WAV_FMT),
                SEAL_BUF_INUSE);
    ASSERT_FAIL(!seal_free_buf(buf), SEAL_BUF_INUSE);
    ASSERT_FAIL(!seal_set_src_stream(src, stream), SEAL_MIXING_SRC_TYPE);

    seal_stop_src(src);
    ASSERT_NO_ERR();
    ASSERT_OK(seal_set_src_buf(src, buf));
    ASSERT_OK(seal_get_src_type(src) == SEAL_STATIC);
    ASSERT_FAIL(!seal_load2buf(buf, TEST_FILENAME, SEAL_WAV_FMT),
                SEAL_BUF_INUSE);
    ASSERT_FAIL(!seal_free_buf(buf), SEAL_BUF_INUSE);
    ASSERT_FAIL(!seal_set_src_stream(src, stream), SEAL_MIXING_SRC_TYPE);

    ASSERT_OK(seal_get_src_buf(src) == buf);
    seal_detach_src_audio(src);
    ASSERT_OK(seal_get_src_type(src) == SEAL_UNDETERMINED);
    ASSERT_OK(seal_load2buf(buf, TEST_FILENAME, SEAL_WAV_FMT));
    ASSERT_OK(seal_free_buf(buf));
    ASSERT_OK(seal_set_src_stream(src, stream));
    ASSERT_OK(seal_get_src_type(src) == SEAL_STREAMING);
    seal_detach_src_audio(src);
    ASSERT_OK(seal_get_src_type(src) == SEAL_UNDETERMINED);

    assert_new_buf_ok(&buf);
    ASSERT_OK(seal_set_src_buf(src, buf));
    ASSERT_OK(seal_get_src_type(src) == SEAL_STATIC);
    seal_free_src(src);
    ASSERT_OK(seal_load2buf(buf, TEST_FILENAME, SEAL_WAV_FMT));
    ASSERT_OK(seal_free_buf(buf));
    ASSERT_OK(seal_free_stream(stream));

cleanup:
    seal_cleanup();
    remove(TEST_FILENAME);
}

void
test_src_stream(void)
{
    seal_src_t* src;
    seal_src_t* src2;
    seal_buf_t* buf;
    seal_stream_t* stream;
    seal_stream_t* stream2;

    if (setjmp(next_test)) goto cleanup;

    generate_test_file();
    ASSERT_OK(seal_startup(0));
    assert_init_ok(&src, &buf, &stream);

    ASSERT_FAIL(seal_init_stream(stream, TEST_FILENAME, SEAL_WAV_FMT) == 0,
                SEAL_STREAM_ALREADY_OPENED);

    ASSERT_OK(seal_set_src_stream(src, stream));
    ASSERT_OK(seal_get_src_type(src) == SEAL_STREAMING);

    ASSERT_FAIL(!seal_close_stream(stream), SEAL_STREAM_INUSE);
    ASSERT_FAIL(!seal_free_stream(stream), SEAL_STREAM_INUSE);
    /* Test setting the same stream to the source that is using it. */
    ASSERT_OK(seal_set_src_stream(src, stream));
    ASSERT_FAIL(!seal_set_src_buf(src, buf), SEAL_MIXING_SRC_TYPE);
    ASSERT_OK(seal_update_src(src) >= 0);

    seal_detach_src_audio(src);
    ASSERT_OK(seal_get_src_type(src) == SEAL_UNDETERMINED);
    ASSERT_OK(seal_set_src_buf(src, buf));
    ASSERT_OK(seal_close_stream(stream));
    ASSERT_FAIL(!seal_close_stream(stream), SEAL_STREAM_UNOPENED);
    ASSERT_OK(seal_free_stream(stream));

    stream = seal_alloc_stream();
    ASSERT_OK(stream != 0);
    ASSERT_FAIL(!seal_close_stream(stream), SEAL_STREAM_UNOPENED);
    ASSERT_FAIL(!seal_set_src_stream(src, stream), SEAL_MIXING_SRC_TYPE);
    seal_detach_src_audio(src);
    ASSERT_FAIL(!seal_set_src_stream(src, stream), SEAL_STREAM_UNOPENED);
    ASSERT_OK(seal_free_stream(stream));

    assert_alloc_src_ok(&src2);
    assert_open_stream_ok(&stream);
    assert_open_stream_ok(&stream2);
    ASSERT_OK(seal_set_src_stream(src, stream));
    ASSERT_OK(seal_set_src_stream(src, stream2));
    ASSERT_FAIL(!seal_set_src_stream(src2, stream2), SEAL_STREAM_INUSE);
    ASSERT_OK(seal_set_src_stream(src2, stream));
    ++stream->attr.freq;
    ASSERT_FAIL(!seal_set_src_stream(src, stream), SEAL_STREAM_INUSE);
    seal_detach_src_audio(src2);
    ASSERT_FAIL(!seal_set_src_stream(src, stream), SEAL_MIXING_STREAM_FMT);
    ASSERT_OK(seal_free_stream(stream));
    seal_free_src(src);
    ASSERT_OK(seal_free_stream(stream2));
    seal_free_src(src2);
    ASSERT_OK(seal_free_buf(buf));

cleanup:
    seal_cleanup();
    remove(TEST_FILENAME);
}

void
test_src_simple_attr(void)
{
    seal_src_t* src;
    float pos[] = { 0.2f, 45.3f, -4.5f };
    float actual_pos[] = { 0.0f, 0.0f, 0.0f };
    float default_pos[] = { 0.0f, 0.0f, 0.0f };
    float vel[] = { 3.0f, -13.445f, 0 };
    float default_vel[] = { 0.0f, 0.0f, 0.0f };
    float actual_vel[] = { 0.0f, 0.0f, 0.0f };

    if (setjmp(next_test)) goto cleanup;

    ASSERT_OK(seal_startup(0));
    assert_alloc_src_ok(&src);

    ASSERT_OK(seal_get_src_queue_size(src) == 3);
    ASSERT_FAIL(!seal_set_src_queue_size(src, 0), SEAL_BAD_SRC_ATTR_VAL);
    ASSERT_FAIL(!seal_set_src_queue_size(src, 1), SEAL_BAD_SRC_ATTR_VAL);
    ASSERT_OK(seal_set_src_queue_size(src, 2));
    ASSERT_OK(seal_set_src_queue_size(src, 63));
    ASSERT_OK(seal_set_src_queue_size(src, 32));
    ASSERT_FAIL(!seal_set_src_queue_size(src, 64), SEAL_BAD_SRC_ATTR_VAL);
    ASSERT_FAIL(!seal_set_src_queue_size(src, 321428), SEAL_BAD_SRC_ATTR_VAL);
    ASSERT_OK(seal_get_src_queue_size(src) == 32);

    ASSERT_OK(seal_get_src_chunk_size(src) == 36864);
    ASSERT_FAIL(!seal_set_src_chunk_size(src, 0), SEAL_BAD_SRC_ATTR_VAL);
    ASSERT_FAIL(!seal_set_src_chunk_size(src, 432), SEAL_BAD_SRC_ATTR_VAL);
    ASSERT_FAIL(!seal_set_src_chunk_size(src, 9215), SEAL_BAD_SRC_ATTR_VAL);
    ASSERT_OK(seal_set_src_chunk_size(src, 9216));
    ASSERT_FAIL(!seal_set_src_chunk_size(src, 9217), SEAL_BAD_SRC_ATTR_VAL);
    ASSERT_FAIL(!seal_set_src_chunk_size(src, 32768), SEAL_BAD_SRC_ATTR_VAL);
    ASSERT_OK(seal_set_src_chunk_size(src, 294912));
    ASSERT_FAIL(!seal_set_src_chunk_size(src, 16773119),
                SEAL_BAD_SRC_ATTR_VAL);
    ASSERT_OK(seal_set_src_chunk_size(src, 16773120));
    ASSERT_FAIL(!seal_set_src_chunk_size(src, 16773121),
                SEAL_BAD_SRC_ATTR_VAL);
    ASSERT_OK(seal_get_src_chunk_size(src) == 16773120);

    seal_get_src_pos(src, actual_pos, actual_pos + 1, actual_pos + 2);
    ASSERT(memcmp(default_pos, actual_pos, sizeof (actual_pos)) == 0);
    seal_set_src_pos(src, pos[0], pos[1], pos[2]);
    seal_get_src_pos(src, actual_pos, actual_pos + 1, actual_pos + 2);
    ASSERT(memcmp(pos, actual_pos, sizeof (actual_pos)) == 0);

    seal_get_src_vel(src, actual_vel, actual_vel + 1, actual_vel + 2);
    ASSERT(memcmp(default_vel, actual_vel, sizeof (actual_vel)) == 0);
    seal_set_src_vel(src, vel[0], vel[1], vel[2]);
    seal_get_src_vel(src, actual_vel, actual_vel + 1, actual_vel + 2);
    ASSERT(memcmp(vel, actual_vel, sizeof (actual_vel)) == 0);

    ASSERT_OK(seal_get_src_pitch(src) == 1.0f);
    ASSERT_OK(seal_set_src_pitch(src, 2.1903f));
    ASSERT_FAIL(!seal_set_src_pitch(src, -3.1f), SEAL_BAD_SRC_ATTR_VAL);
    ASSERT_OK(seal_get_src_pitch(src) == 2.1903f);

    ASSERT_OK(seal_get_src_gain(src) == 1.0f);
    ASSERT_OK(seal_set_src_gain(src, 32.01f));
    ASSERT_FAIL(!seal_set_src_gain(src, -1.13f), SEAL_BAD_SRC_ATTR_VAL);
    ASSERT_OK(seal_get_src_gain(src) == 32.01f);

    ASSERT_OK(seal_is_src_relative(src) == 0);
    ASSERT_OK(seal_set_src_relative(src, 1));
    ASSERT_OK(seal_is_src_relative(src));
    ASSERT_OK(seal_set_src_relative(src, 0));
    ASSERT_OK(!seal_is_src_relative(src));

    seal_free_src(src);

cleanup:
    seal_cleanup();
}

void
test_src_looping(void)
{
    seal_src_t* src;
    seal_buf_t* buf;
    seal_stream_t* stream;

    if (setjmp(next_test)) goto cleanup;

    ASSERT_OK(seal_startup(0));
    generate_test_file();
    assert_init_ok(&src, &buf, &stream);

    /* `SEAL_UNDETERMINED' type. */
    ASSERT_OK(!seal_is_src_looping(src));
    ASSERT_OK(seal_set_src_looping(src, 1));
    ASSERT_OK(seal_is_src_looping(src));
    ASSERT_OK(seal_set_src_looping(src, 0));
    ASSERT_FAIL(!seal_set_src_looping(src, 2), SEAL_BAD_SRC_ATTR_VAL);
    ASSERT_OK(!seal_is_src_looping(src));
    ASSERT_OK(seal_set_src_looping(src, 1));

    /* `SEAL_STEAMING' type. */
    ASSERT_OK(seal_set_src_stream(src, stream));
    ASSERT_OK(seal_is_src_looping(src));
    ASSERT_OK(seal_set_src_looping(src, 0));
    ASSERT_OK(!seal_is_src_looping(src));
    ASSERT_OK(seal_set_src_looping(src, 1));

    /* `seal_update_src' will never return 0 for looping sources. */
    ASSERT_OK(seal_play_src(src));
    _seal_sleep(500);
    ASSERT_OK(seal_update_src(src) > 0);

    /* `SEAL_STATIC' type. */
    seal_detach_src_audio(src);
    ASSERT_OK(seal_set_src_buf(src, buf));
    ASSERT_OK(seal_is_src_looping(src));
    ASSERT_OK(seal_set_src_looping(src, 0));
    ASSERT_OK(!seal_is_src_looping(src));

    /* `SEAL_UNDETERMINED' type. */
    seal_detach_src_audio(src);
    ASSERT_OK(!seal_is_src_looping(src));

    seal_free_src(src);
    seal_free_buf(buf);
    seal_free_stream(stream);

cleanup:
    seal_cleanup();
    remove(TEST_FILENAME);
}

void
test_src_state(void)
{
    seal_src_t* src;
    seal_stream_t* stream;

    if (setjmp(next_test)) goto cleanup;

    ASSERT_OK(seal_startup(0));
    generate_test_file();
    assert_alloc_src_ok(&src);
    assert_open_stream_ok(&stream);

    /* Transition from `SEAL_INITIAL'. */
    ASSERT_OK(seal_get_src_state(src) == SEAL_INITIAL);
    ASSERT_OK(seal_set_src_stream(src, stream));
    seal_stop_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_INITIAL);
    seal_pause_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_INITIAL);
    seal_rewind_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_INITIAL);
    ASSERT_OK(seal_play_src(src));
    ASSERT_OK(seal_get_src_state(src) == SEAL_PLAYING);

    /* Transition from `SEAL_PLAYING'. */
    ASSERT_OK(seal_play_src(src));
    ASSERT_OK(seal_get_src_state(src) == SEAL_PLAYING);
    seal_rewind_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_INITIAL);
    ASSERT_OK(seal_play_src(src));
    ASSERT_OK(seal_get_src_state(src) == SEAL_PLAYING);
    seal_stop_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_STOPPED);
    ASSERT_OK(seal_play_src(src));
    ASSERT_OK(seal_get_src_state(src) == SEAL_PLAYING);
    seal_pause_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_PAUSED);

    /* Transition from `SEAL_PAUSED'. */
    seal_pause_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_PAUSED);
    ASSERT_OK(seal_play_src(src));
    ASSERT_OK(seal_get_src_state(src) == SEAL_PLAYING);
    seal_pause_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_PAUSED);
    seal_rewind_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_INITIAL);
    ASSERT_OK(seal_play_src(src));
    ASSERT_OK(seal_get_src_state(src) == SEAL_PLAYING);
    seal_pause_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_PAUSED);
    seal_stop_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_STOPPED);

    /* Transition from `SEAL_STOPPED'. */
    seal_stop_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_STOPPED);
    seal_pause_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_STOPPED);
    ASSERT_OK(seal_play_src(src));
    ASSERT_OK(seal_get_src_state(src) == SEAL_PLAYING);
    seal_stop_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_STOPPED);
    seal_rewind_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_INITIAL);

    /* `seal_detach_src_audio' should set source state to `SEAL_INITIAL'. */
    /* Detach from `SEAL_INITIAL'. */
    seal_detach_src_audio(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_INITIAL);
    ASSERT_OK(seal_get_src_type(src) == SEAL_UNDETERMINED);
    seal_detach_src_audio(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_INITIAL);

    /* Detach from `SEAL_PLAYING'. */
    seal_rewind_stream(stream);
    ASSERT_OK(seal_set_src_stream(src, stream));
    ASSERT_OK(seal_play_src(src));
    ASSERT_OK(seal_get_src_state(src) == SEAL_PLAYING);
    seal_detach_src_audio(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_INITIAL);

    /* Detach from `SEAL_PAUSED'. */
    seal_rewind_stream(stream);
    ASSERT_OK(seal_set_src_stream(src, stream));
    ASSERT_OK(seal_play_src(src));
    ASSERT_OK(seal_get_src_state(src) == SEAL_PLAYING);
    seal_pause_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_PAUSED);
    seal_detach_src_audio(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_INITIAL);

    /* Detach from `SEAL_STOPPED'. */
    seal_rewind_stream(stream);
    ASSERT_OK(seal_set_src_stream(src, stream));
    ASSERT_OK(seal_play_src(src));
    ASSERT_OK(seal_get_src_state(src) == SEAL_PLAYING);
    seal_stop_src(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_STOPPED);
    seal_detach_src_audio(src);
    ASSERT_OK(seal_get_src_state(src) == SEAL_INITIAL);

    seal_free_src(src);
    seal_free_stream(stream);

cleanup:
    seal_cleanup();
    remove(TEST_FILENAME);
}

void
test_src(void)
{
    test_src_buf();
    test_src_stream();
    test_src_simple_attr();
    test_src_looping();
    test_src_state();
}