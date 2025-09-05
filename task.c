#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define WIDTH 50.000

struct plane_limits
{
    float x0;
    float y0;
    float x1;
    float y1;
};

struct point
{
    float x;
    float y;
};

struct rectangle
{
    struct point A;  // Corner A
    struct point B;  // Corner B  
    struct point C;  // Corner C
    struct point D;  // Corner D
};

struct interval {
    float start, end;
};

// Convert line segment to rectangle with corners
struct rectangle get_rectangle_from_line(struct plane_limits plane, float width)
{
    float dx = plane.x1 - plane.x0;
    float dy = plane.y1 - plane.y0;
    float length = sqrt(dx*dx + dy*dy);
    
    // Normalize the direction vector
    float nx = dx / length;
    float ny = dy / length;
    
    // Perpendicular vector (rotated 90 degrees)
    float px = -ny;
    float py = nx;
    
    // Half width offset
    float offset_x = px * width;
    float offset_y = py * width;
    
    struct rectangle rec;
    
    // Calculate the four corners
    // A and B are at the start of the line, C and D at the end
    rec.A.x = plane.x0 + offset_x;
    rec.A.y = plane.y0 + offset_y;
    
    rec.B.x = plane.x1 + offset_x;
    rec.B.y = plane.y1 + offset_y;
    
    rec.C.x = plane.x1 - offset_x;
    rec.C.y = plane.y1 - offset_y;
    
    rec.D.x = plane.x0 - offset_x;
    rec.D.y = plane.y0 - offset_y;
    
    return rec;
}

// Calculate x-interval for rectangle at given y-coordinate
void calculate_x_interval_at_y(struct rectangle *rect, float y, float *x_start, float *x_end)
{
    float x_coords[4];
    int valid_count = 0;
    
    // Check intersection of horizontal line y with each edge of rectangle
    struct point corners[4] = {rect->A, rect->B, rect->C, rect->D};
    
    for (int i = 0; i < 4; i++)
    {
        struct point p1 = corners[i];
        struct point p2 = corners[(i + 1) % 4];
        
        // Check if horizontal line intersects this edge
        float min_y = fmin(p1.y, p2.y);
        float max_y = fmax(p1.y, p2.y);
        
        if (y >= min_y && y <= max_y)
        {
            if (fabs(p2.y - p1.y) < 1e-3)
            {
                // Horizontal edge - add both x coordinates
                if (valid_count < 4) x_coords[valid_count++] = p1.x;
                if (valid_count < 4) x_coords[valid_count++] = p2.x;
            } 
            else
            {
                // Non-horizontal edge - calculate intersection
                float t = (y - p1.y) / (p2.y - p1.y);
                float x_intersect = p1.x + t * (p2.x - p1.x);
                if (valid_count < 4) x_coords[valid_count++] = x_intersect;
            }
        }
    }
    
    // Find min and max x coordinates
    if (valid_count > 0) {
        *x_start = x_coords[0];
        *x_end = x_coords[0];
        for (int i = 1; i < valid_count; i++) {
            *x_start = fmin(*x_start, x_coords[i]);
            *x_end = fmax(*x_end, x_coords[i]);
        }
    } else {
        *x_start = 0;
        *x_end = 0;
    }
}

// Compare function for sorting intervals
int compare_intervals(const void *a, const void *b)
{
    struct interval *ia = (struct interval*)a;
    struct interval *ib = (struct interval*)b;
    if (ia->start < ib->start) return -1;
    if (ia->start > ib->start) return 1;
    return 0;
}

// Check if intervals cover [0, map_size] and find gap location if exists
bool intervals_cover_range_with_gap(struct interval *v, int n, float map_size, float *gap_x) {
    const float EPS = 1e-3f;
    qsort(v, n, sizeof(*v), compare_intervals);
    float cur = 0.0f;
    int i = 0;
    while (cur < map_size - EPS) 
    {
        if (i >= n || v[i].start > cur + EPS) 
        { 
            *gap_x = cur; 
            return false;
        }
        float best = cur;
        while (i < n && v[i].start <= cur + EPS) 
        { 
            best = fmaxf(best, v[i].end); 
            i++;
        }
        if (best <= cur + EPS) 
        { 
            *gap_x = cur;
            return false;
        }
        cur = best;
    }
    return true;
}


bool check_full_coverage_optimized(struct rectangle *rectangles, int planes, int map_size, float *gap_x, float *gap_y)
{
    // For each y-coordinate (integer), compute x-intervals covered by rectangles
    for (float y = 0; y < map_size; y+=0.001)
    {
        struct interval y_intervals[100];  // Max 100 planes
        int interval_count = 0;
        
        // Find which rectangles intersect this y-coordinate
        for (int p = 0; p < planes; p++)
        {
            float min_y = fmin(fmin(rectangles[p].A.y, rectangles[p].B.y), 
                              fmin(rectangles[p].C.y, rectangles[p].D.y));
            float max_y = fmax(fmax(rectangles[p].A.y, rectangles[p].B.y), 
                              fmax(rectangles[p].C.y, rectangles[p].D.y));
            
            if (y >= min_y && y <= max_y)
            {
                // Calculate x-interval for this rectangle at y-coordinate
                float x_start, x_end;
                calculate_x_interval_at_y(&rectangles[p], y, &x_start, &x_end);
                
                y_intervals[interval_count].start = x_start;
                y_intervals[interval_count].end = x_end;
                interval_count++;
            }
        }
        
        // Check if intervals cover [0, map_size] for this y and find gap location
        if (!intervals_cover_range_with_gap(y_intervals, interval_count, map_size, gap_x))
        {
            if (y == 0)
            {
                *gap_y = y;
            }
            else 
            {
                *gap_y = y+0.001;
            }
            return false;  // Gap found at y-coordinate
        }
    }
    
    return true;  // Full coverage
}

int main(int argc, char* argv[])
{
    int map = 0;
    int planes = 0;
    float x0 = 0.0f, y0 = 0.0f, x1 = 0.0f, y1 = 0.0f;
    FILE* fp, *fp2;
    clock_t start, end;

    start = clock();
    if(argc > 1)
    {
        fp = fopen(argv[1], "r");
    }
    else
    {
        fp = fopen("INPUT.txt","r");
    }
    fp2 = fopen("OUTPUT.txt","w");
    
    if (fp == NULL)
    {
        perror("ERROR\n");
        return 1;
    }
    
    //Read the map size 
    fscanf(fp,"%d", &map);
    if(map > 1000 || map < 1)
    {
        fprintf(fp2,"ERROR\n");
        fclose(fp2);
        return 1;
    }
    
    // Read the airplane number
    fscanf(fp,"%d", &planes);
    if(planes > 100 || planes < 1)
    {
        fprintf(fp2,"ERROR\n");
        fclose(fp2);
        return 1;
    }
    
    // Start reading planes report 
    int cont = 0;
    struct plane_limits plane[planes];
    struct rectangle rec[planes];
    
    if (argc > 1) // Means debug
    {
        printf("Map size: %d\n", map);
    }
    while(cont < planes)
    {
        fscanf(fp,"%f %f %f %f",&x0,&y0,&x1,&y1);
        plane[cont].x0 = x0;
        plane[cont].x1 = x1;
        plane[cont].y0 = y0;
        plane[cont].y1 = y1;
        rec[cont] = get_rectangle_from_line(plane[cont], WIDTH);
        
        // if the argc > 1, this means its running the tests. so print verbosity
        if(argc > 1)
        {
            printf("Plane %d: Line from (%.3f, %.3f) to (%.3f, %.3f)\n",cont, x0, y0, x1, y1);
            printf("Rectangle corners:\n");
            printf("  A: (%.3f, %.3f)\n", rec[cont].A.x, rec[cont].A.y);
            printf("  B: (%.3f, %.3f)\n", rec[cont].B.x, rec[cont].B.y);
            printf("  C: (%.3f, %.3f)\n", rec[cont].C.x, rec[cont].C.y);
            printf("  D: (%.3f, %.3f)\n", rec[cont].D.x, rec[cont].D.y);
            printf("\n");
        }
        cont++;
    }
    fclose(fp);
    
    // Use optimized coverage check to find gap
    float gap_x, gap_y;
    if (check_full_coverage_optimized(rec, planes, map, &gap_x, &gap_y)) {
        fprintf(fp2, "OK\n");
    } else {
        // Gap found - output the coordinates
        fprintf(fp2, "%.3f %.3f\n", gap_x, gap_y);
    }
    
    fclose(fp2);
    end = clock();
    double run_time = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Total run time: %f seconds\n",run_time);
    return 0;
}
