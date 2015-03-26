/* Copyright 2014,2015 Bas van den Berg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#ifdef _WIN32
#include <fstream>
#else
#include <unistd.h>
#include <sys/mman.h>
#endif

#include "FileMap.h"

using namespace Test;

File::File(const char* filename_)
    : filename (filename_)
    , region(0)
    , size(0)
    , last_change(0)
    , write_cache(false)
{}


FileMap::FileMap(const char* filename_)
    : File(filename_)
{
#ifdef _WIN32
    std::ifstream file;

    file.open(filename_);

    file.seekg(0, std::ios::end);
    size = file.tellg();
    file.seekg(0, std::ios::beg);

    region = new char[size];
    file.read((char*)region, size);
    file.close();
#else
    struct stat statbuf;
    int err = stat(filename_, &statbuf);
    if (err) {
        fprintf(stderr, "cannot stat '%s': %s\n", filename_, strerror(errno));
        exit(-1);
    }
    size = statbuf.st_size;
    last_change = statbuf.st_mtime;
#endif
}


FileMap::~FileMap() {
    if (region) close();
}

void FileMap::open() {
#ifdef _WIN32
    std::ifstream file;

    close();

    file.open(filename);

    file.seekg(0, std::ios::end);
    size = file.tellg();
    file.seekg(0, std::ios::beg);

    region = new char[size];
    file.read((char*)region, size);
    file.close();
    write_cache = true;
#else
    int fd = ::open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
      fprintf(stderr, "error opening '%s': %s\n", filename.c_str(), strerror(errno));
      exit(-1);
    }

    region = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (region == MAP_FAILED) {
      perror("mmap");
      exit(-1);
    }
    ::close (fd);
    write_cache = true;
#endif
}

void FileMap::close() {
#ifdef _WIN32
    if(region)
    {
      delete(region);
    }
    region = 0;
#else
    munmap(region, size);
     region = 0;
#endif
}

FileString::FileString(const char* filename_, const char* text, time_t ts)
    : File(filename_)
{
    region = (void*)text;
    size = strlen(text);
    last_change = ts;
}

