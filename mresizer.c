
#include <gd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[]) {

	DIR *directory;
	struct dirent *entry;

	for(directory = opendir("."); (entry = readdir(directory)) != 0;) {
		if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
		if(strstr(entry->d_name, ".jpg") != entry->d_name + strlen(entry->d_name) - 4) continue;

		FILE *file = fopen(entry->d_name, "r");
		gdImagePtr source = gdImageCreateFromJpeg(file);

		int source_width = gdImageSX(source);
		int source_height = gdImageSY(source);

		int destination_width = source_width / 8;
		int destination_height = source_height / 8;

		gdImagePtr destination = gdImageCreateTrueColor(destination_width, destination_height);

		gdImageCopyResampled(destination, source, 0, 0, 0, 0, destination_width, destination_height, source_width, source_height);

		FILE *destination_file = fopen("dest.jpg", "w");
		fseek(destination_file, 0, SEEK_SET);
		gdImageJpeg(destination, destination_file, 90);
		fclose(destination_file);

		gdImageDestroy(destination);
		gdImageDestroy(source);

		fclose(file);

	}

	closedir(directory);
	return 0;
}
