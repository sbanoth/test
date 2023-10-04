#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 1024
#define MAX_ENTRIES 1000

struct listing {
    int id, host_id, minimum_nights, number_of_reviews, calculated_host_listings_count, availability_365;
    char *host_name, *neighbourhood_group, *neighbourhood, *room_type;
    float latitude, longitude, price;
};

struct listing list_items[MAX_ENTRIES];

struct listing getfields(char* line){
    struct listing item;

    item.id = atoi(strtok(line, ","));
    item.host_id = atoi(strtok(NULL, ","));
    item.host_name = strdup(strtok(NULL, ","));
    item.neighbourhood_group = strdup(strtok(NULL, ","));
    item.neighbourhood = strdup(strtok(NULL, ","));
    item.latitude = atof(strtok(NULL, ","));
    item.longitude = atof(strtok(NULL, ","));
    item.room_type = strdup(strtok(NULL, ","));
    item.price = atof(strtok(NULL, ","));
    item.minimum_nights = atoi(strtok(NULL, ","));
    item.number_of_reviews = atoi(strtok(NULL, ","));
    item.calculated_host_listings_count = atoi(strtok(NULL, ","));
    item.availability_365 = atoi(strtok(NULL, ","));
    
    return item;
}

void displayStruct(struct listing item){
    printf("ID: %d\nHost ID: %d\nHost Name: %s\nNeighbourhood Group: %s\nNeighbourhood: %s\nLatitude: %f\nLongitude: %f\nRoom Type: %s\nPrice: %f\nMinimum Nights: %d\nNumber of Reviews: %d\nCalculated Host Listings Count: %d\nAvailability 365: %d\n\n",
           item.id, item.host_id, item.host_name, item.neighbourhood_group, item.neighbourhood, item.latitude, item.longitude, item.room_type, item.price, item.minimum_nights, item.number_of_reviews, item.calculated_host_listings_count, item.availability_365);
}

int compare_host_name(const void *a, const void *b){
    return strcmp(((struct listing*)a)->host_name, ((struct listing*)b)->host_name);
}

int compare_price(const void *a, const void *b){
    if (((struct listing*)a)->price < ((struct listing*)b)->price) return -1;
    else if (((struct listing*)a)->price > ((struct listing*)b)->price) return 1;
    else return 0;
}

void writeToFile(const char* filename, struct listing* list_items, int count) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error creating output file %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%d,%s,%s,%s,%f,%f,%s,%f,%d,%d,%d,%d\n", list_items[i].id, list_items[i].host_id,
            list_items[i].host_name, list_items[i].neighbourhood_group, list_items[i].neighbourhood,
            list_items[i].latitude, list_items[i].longitude, list_items[i].room_type,
            list_items[i].price, list_items[i].minimum_nights, list_items[i].number_of_reviews,
            list_items[i].calculated_host_listings_count, list_items[i].availability_365);
    }

    fclose(file);
}

int main() {
    FILE *fptr = fopen("listings.csv", "r");
    char line[LINESIZE];
    int count = 0;

    if (fptr == NULL) {
        fprintf(stderr, "Error opening input file listings.csv\n");
        return 1;
    }

    while (fgets(line, LINESIZE, fptr) != NULL) {
        list_items[count++] = getfields(line);
    }

    // Sorting by host name
    qsort(list_items, count, sizeof(struct listing), compare_host_name);

    // Write sorted data to a file
    writeToFile("sorted_by_host_name.csv", list_items, count);

    // Sorting by price
    qsort(list_items, count, sizeof(struct listing), compare_price);

    // Write sorted data to a file
    writeToFile("sorted_by_price.csv", list_items, count);

    fclose(fptr);
    return 0;
}
