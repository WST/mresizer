
#include <gd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>

char destination_filename[255];

int main(int argc, char *argv[]) {

	// Output directory
	mkdir("results", S_IRWXU | S_IRWXG | S_IRWXO);

	// Directory for reading and file entry
	DIR *directory;
	struct dirent *entry;

	// Main loop
	for(directory = opendir("."); (entry = readdir(directory)) != 0;) {
		if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
		if(strstr(entry->d_name, ".jpg") != entry->d_name + strlen(entry->d_name) - 4) continue;
		if(strstr(entry->d_name, ".resized.jpg") == entry->d_name + strlen(entry->d_name) - 12) continue;

		printf("Processing <%s>\n", entry->d_name);

		FILE *file = fopen(entry->d_name, "r");
		gdImagePtr source = gdImageCreateFromJpeg(file);

		int source_width = gdImageSX(source);
		int source_height = gdImageSY(source);

		int destination_width = 1024;
		int destination_height = 768;

		gdImagePtr destination = gdImageCreateTrueColor(destination_width, destination_height);

		gdImageCopyResampled(destination, source, 0, 0, 0, 0, destination_width, destination_height, source_width, source_height);

		sprintf(destination_filename, "results/%s.resized.jpg", entry->d_name);

		FILE *destination_file = fopen(destination_filename, "w");
		fseek(destination_file, 0, SEEK_SET);
		gdImageJpeg(destination, destination_file, 90);
		fclose(destination_file);

		gdImageDestroy(destination);
		gdImageDestroy(source);

		fclose(file);
	}
	closedir(directory);

	printf("Job done!\n");

	return 0;
}
