#pragma once

#define MAGIC ((unsigned char[]){'M', 'R', 'T', 'N'})
#define MAGIC_SIZE 4
#define VERSION 1

int create_mrtn(const char *bin_path, const char *mrtn_path);
int inspect_mrtn(const char *mrtn_path);
