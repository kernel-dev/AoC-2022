#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct {
    //
    //  Collective amount of calories this
    //  Elf is carrying.
    //
    uint64_t calories;

    //
    //  What Elf we are talking about.
    //
    //  Surely 2^16 is enough elves...
    //
    uint16_t index;
} Elf;

//
//  Auxiliary function to check if
//  an item is inside of the specified array.
//
int elf_exists_by_index(uint16_t index, Elf *elves, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        if (elves[i].index == index)
            return 1;
    }

    return 0;
}

//
//  Sorts an Elf array in ascending
//  order, according to each Elf's
//  `calories` property.
//
void sort_elves(Elf *elves, size_t size)
{
    int changes = 0;
    Elf *temporary = (Elf *)malloc(sizeof(Elf) * size);

    memcpy(temporary, elves, sizeof(Elf) * size);

    for (int i = 0; i < size; i++)
    {
        //
        //  Keep track of the Elf with the
        //  lowest amount of calories.
        //
        Elf *lowest = &elves[0];
        uint16_t ind = 0;

        //
        //  Current iteration elf.
        //
        Elf *current = &elves[i];

        for (int j = 0; j < size; j++)
        {
            //
            //  We found an elf with the lowest
            //  calorie count!
            //
            if (
                current->index != lowest->index && 
                current->calories < lowest->calories
            )
            {
                lowest = current;
                ind = j;
                changes = 1;
            }
        }

        //
        //  Put lowest elf at the first position,
        //  and the element at the first position into
        //  the previous position of the lowest element's. 
        //
        elves[ind] = elves[0];
        elves[0] = *lowest;
    }

    if (changes != 0)
        return sort_elves(elves, size);
}

int main(void)
{
    FILE *opened = fopen("input.txt", "r+");

    //
    //  Couldn't open file.
    //
    if (opened == NULL)
    {
        printf("[SOLUTION::INTERNAL_ERROR]: Failed to open input file.");

        return 1;
    }

    char *line = NULL;
    size_t buf_size = 0;
    ssize_t read;

    //
    //  Size of "top 3" array.
    //
    size_t size = 3;
    Elf *elves = (Elf *)malloc(sizeof(Elf) * size);
    uint64_t latest = 0;
    uint64_t index = 1;

    while ((read = getline(&line, &buf_size, opened)) != -1)
    {
        if (strcmp(line, "\n") == 0)
        {
            for (int i = 0; i < size; i++)
            {
                //
                //  If this elf doesn't exist, and its
                //  calorie value is greater than the current elf's
                //  calorie value; replace the current with the new elf.
                //
                if (
                    !elf_exists_by_index(index, elves, size) &&
                    elves[i].calories < latest
                )
                {
                    elves[i] = (Elf){ .calories = latest, .index = index };

                    //
                    //  Sort Elf array.
                    //
                    sort_elves(elves, size);
                }
            }

            latest = 0;
            index++;
        }

        else
            //
            //  Remove suffixed "\n" from string.
            //
            if (strstr(line, "\n") == 0) {
                line[strlen(line) - 1] = '\0';
            }

            latest += atoi(line);
    }

    printf("Solution:\n\n");
    printf(
        "Top 3 Elves: {\n\t#%u: %llu, \n\t#%u: %llu, \n\t#%u: %llu\n}\n",
        elves[0].index, elves[0].calories,
        elves[1].index, elves[1].calories,
        elves[2].index, elves[2].calories
    );
    printf(
        "Combined calories: %llu\n", 
        elves[0].calories + elves[1].calories + elves[2].calories
    );

    fclose(opened);

    return 0;
}