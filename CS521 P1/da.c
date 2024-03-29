WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "sys/stat.h"
#include "util.h"
#include "logger.h"
#include "elist.h"
#include <sys/ioctl.h>


/* Forward declarations: */
void print_usage(char *argv[]);
ssize_t elist_add (struct elist *list, void *item);

struct elist {
   size_t capacity;         /*!< Storage space allocated for list items */
    size_t size;             /*!< The actual number of items in the list */
    size_t item_sz;          /*!< Size of the items stored in the list */
    void* element_storage;   /*!< Pointer to the beginning of the array */
};

struct file_info
    {
        char filepath[256];
        double size;
        time_t time;
    };


int cmp_int_s(const void* _a, const void* _b)
{
       struct file_info* a = (struct file_info*)_a;
    struct file_info* b = (struct file_info*)_b;
    return a->size < b->size;
}

int cmp_int_t(const void* _a, const void* _b)
{
	struct file_info* a = (struct file_info*)_a;
	struct file_info* b = (struct file_info*)_b;
	return  memcmp(&a->time,&b->time, 15);
}

void print_usage(char *argv[]) {
fprintf(stderr, "Disk Analyzer (da): analyzes disk space usage\n");
fprintf(stderr, "Usage: %s [-ahs] [-l limit] [directory]\n\n", argv[0]);

fprintf(stderr, "If no directory is specified, the current working directory is used.\n\n");

fprintf(stderr, "Options:\n"
"    * -a              Sort the files by time of last access (descending)\n"
"    * -h              Display help/usage information\n"
"    * -l limit        Limit the output to top N files (default=unlimited)\n"
"    * -s              Sort the files by size (default, ascending)\n\n"
);
}


void readFileList(const char* directory, struct elist *list)
{
	//build up a struct to store file info
    struct file_info f;
    DIR* dir;
    struct dirent* ptr;
    //use stat to retrieve access times and files sizes
    struct stat statbuf;
    if ((dir = opendir(directory)) == NULL)
    {
	    perror("opendir");
	    return;
	 }
        char* path = malloc(sizeof(char) * 256);
	// list all files and store them in a linkedlist
        while ((ptr = readdir(dir)) != NULL)
        {
            if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir
                continue;

            strcpy(path, directory);
            strcat(path, "/");
            strcat(path, ptr->d_name);
	    //check the type of current file, if it is a regular file, add it to the list 
	    if(ptr-> d_type == DT_REG)
            {
                if (stat(path, &statbuf) <0)
                {
                    printf("Get stat on Error：%s\n", path);
                    free(path);
                }
                else
                {
                    strcpy(f.filepath, path);
                    LOG("path is:%s\n", f.filepath);
                    f.size = statbuf.st_size;
                    f.time= statbuf.st_atime;
                    elist_add(list, &f);
                    memset(path, 0, strlen(path));
                    LOG("size is:%f\n", f.size);
                    LOG("time is:%ld\n", f.time);
                }
            }
	    // if type of current file has subdirectory, recursive call this function to keep traversing
            else if (ptr->d_type == DT_DIR) 
            {
                readFileList(path,list);
            }
        }

        closedir(dir);
        free(path);
}

int main(int argc, char *argv[])
{
    /* Create a struct to hold program options and initialize it by declaring an
     * 'options' variable. Defaults:
     *      - sort by size (time=false)
     *      - limit of 0 (unlimited)
     *      - directory = '.' (current directory) */
struct da_options {
        bool sort_by_time;
        unsigned int limit;
        char* directory;
    } options
        = { false, 0, "." };

    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "ahl:s")) != -1) {
        switch (c) {
        case 'a':
            options.sort_by_time = true;
            break;
        case 'h':
            print_usage(argv);
            return 0;
        case 's':
            options.sort_by_time = false;
            break;
        case 'l': {
            /*    ^-- to declare 'endptr' here we need to enclose this case
             *    in its own scope with curly braces { } */
            char* endptr;
            long llimit = strtol(optarg, &endptr, 10);
            if (llimit < 0 || llimit > INT_MAX || endptr == optarg) {
                fprintf(stderr, "Invalid limit: %s\n", optarg);
                print_usage(argv);
                return 1;
            }
            options.limit = (int)llimit;
            break;
        }
        case '?':
            if (optopt == 'l') {
                fprintf(stderr,
                    "Option -%c requires an argument.\n", optopt);
            }
            else if (isprint(optopt)) {
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            }
            else {
                fprintf(stderr,
                    "Unknown option character `\\x%x'.\n", optopt);
            }
            print_usage(argv);
            return 1;
        default:
            abort();
        }
    }

    if (optind < argc) {
        options.directory = argv[optind];
        LOGP("Done parsing arguments.\n");
        LOG("Sorting by: [%s], limit: [%u]\n",
            options.sort_by_time == true ? "time" : "size",
            options.limit);
        LOG("Directory to analyze: [%s]\n", options.directory);

            }
        size_t size = sizeof(struct file_info);
        struct elist* list = elist_create(1, size);
        readFileList(options.directory, list);
        if(options.sort_by_time){
		elist_sort(list, cmp_int_t);
	}
	else{
		elist_sort(list, cmp_int_s);
	}
	
	int limit;
	if(options.limit > 0){
		limit = options.limit;
	}
	else{
		limit = list->size;
	}

        for (int i = 0; i < limit; i++)
        {
            struct file_info* f = (struct file_info*)elist_get(list, i);
	    LOG("%s\n", f->filepath);
	    char size_buffer[14];
            human_readable_size(size_buffer,14,f->size, 1);
	    LOG("%s\n", size_buffer);
	    char time_buffer[15];
            simple_time_format(time_buffer, 15, f->time);
	    LOG("%s\n", time_buffer);
	    unsigned short cols = 80;
	    struct winsize win_sz;
            if (ioctl(fileno(stdout), TIOCGWINSZ, &win_sz) != -1) {
                cols = win_sz.ws_col;
            }
	    LOG("Display columns: %d\n", cols);
	    if(strlen(f->filepath) > 51){
		    char temp[51];
		    strcpy(temp, f->filepath + strlen(f->filepath)-51);
		    temp[0] = '.';
		    temp[1] = '.';
		    temp[2] = '.';
		    printf("%51s%14s%15s\n", temp, size_buffer, time_buffer);
		    fflush(stdout);
		    continue;
	    }
            printf( "%51s%14s%15s\n", f->filepath,size_buffer,time_buffer);
	    fflush(stdout);
        }
	elist_destroy(list);

    /* TODO:
     *  - check to ensure the directory actually exists
     *  - create a new 'elist' data structure
     *  - traverse the directory and store entries in the list
     *  - sort the list (either by size or time)
     *  - print formatted list
     */
  return 0;  
}
