#include "oslabs.h"
#include <stdlib.h>

int isNULLRCB(struct RCB request);
int isNULLRCB(struct RCB request) {

    if (request.request_id == 0 && request.arrival_timestamp == 0 && request.cylinder == 0 && request.address == 0 && request.process_id == 0) {
        return 1;
    }

    return 0;
}

struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {

    if(isNULLRCB(current_request)) {
        return new_request;
    } else {
        request_queue[*queue_cnt] = new_request;
        (*queue_cnt)++;
        return current_request;
    }
}

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {

    if(isNULLRCB(current_request)) {
        return new_request;
    } else {
        request_queue[*queue_cnt] = new_request;
        (*queue_cnt)++;
        return current_request;
    }
}

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {

    if(isNULLRCB(current_request)) {
        return new_request;
    } else {
        request_queue[*queue_cnt] = new_request;
        (*queue_cnt)++;
        return current_request;
    }
}

struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt) {

    struct RCB NULLRCB, tmp;
    int i, shortest_arrival_time, shortest_arrival_location;

    if(*queue_cnt == 0) {
        NULLRCB.request_id = 0;
        NULLRCB.arrival_timestamp = 0;
        NULLRCB.cylinder = 0;
        NULLRCB.address = 0;
        NULLRCB.process_id = 0;

        return NULLRCB;
    }

    shortest_arrival_time = request_queue[0].arrival_timestamp;
    shortest_arrival_location = 0;
    for(i = 1; i < *queue_cnt; i++) {
        if(request_queue[i].arrival_timestamp < shortest_arrival_time) {
            shortest_arrival_time = request_queue[i].arrival_timestamp;
            shortest_arrival_location = i;
        }
    }

    tmp = request_queue[shortest_arrival_location];
    for(i = shortest_arrival_location; i < *queue_cnt-1; i++) {
        request_queue[i] = request_queue[i+1];
    }
    (*queue_cnt)--;
    return tmp;
}

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, int current_cylinder, int scan_direction) {

    struct RCB NULLRCB, tmp;
    int i, first_guess, return_rcb, shortest_arrival_time, shortest_arrival_location, closest_cylinder_size, closest_cylinder_location;

    if(*queue_cnt == 0) {
        NULLRCB.request_id = 0;
        NULLRCB.arrival_timestamp = 0;
        NULLRCB.cylinder = 0;
        NULLRCB.address = 0;
        NULLRCB.process_id = 0;

        return NULLRCB;
    }

    first_guess = 1;
    shortest_arrival_location = -1;
    for(i = 0; i < *queue_cnt; i++) {
        if(request_queue[i].cylinder == current_cylinder) {
            if(first_guess) {
                shortest_arrival_location = i;
                shortest_arrival_time = request_queue[i].arrival_timestamp;
                first_guess = 0;
            } else if(request_queue[i].arrival_timestamp < shortest_arrival_time) {
                shortest_arrival_location = i;
                shortest_arrival_time = request_queue[i].arrival_timestamp;
            }
        }
    }

    if(shortest_arrival_location >= 0) {
        tmp = request_queue[shortest_arrival_location];
        for(i = shortest_arrival_location; i < *queue_cnt-1; i++) {
            request_queue[i] = request_queue[i+1];
        }
        (*queue_cnt)--;
        return tmp;
    }

    if(scan_direction == 1) {
        first_guess = 1;
        closest_cylinder_location = -1;
        for(i = 0; i < *queue_cnt; i++) {
            if(request_queue[i].cylinder > current_cylinder) {
                if(first_guess) {
                    closest_cylinder_location = i;
                    closest_cylinder_size = request_queue[i].cylinder;
                    first_guess = 0;
                } else if(request_queue[i].cylinder < closest_cylinder_size) {
                    closest_cylinder_location = i;
                    closest_cylinder_size = request_queue[i].cylinder;
                }
            }
        }

        if(closest_cylinder_location >= 0) {
            tmp = request_queue[closest_cylinder_location];
            for(i = closest_cylinder_location; i < *queue_cnt-1; i++) {
                request_queue[i] = request_queue[i+1];
            }
            (*queue_cnt)--;
            return tmp;
        }

        closest_cylinder_location = 0;
        closest_cylinder_size = request_queue[0].cylinder;
        for(i = 1; i < *queue_cnt; i++) {
            if(request_queue[i].cylinder > closest_cylinder_size) {
                closest_cylinder_location = i;
                closest_cylinder_size = request_queue[i].cylinder;
            }
        }

        tmp = request_queue[closest_cylinder_location];
        for(i = closest_cylinder_location; i < *queue_cnt-1; i++) {
            request_queue[i] = request_queue[i+1];
        }
        (*queue_cnt)--;
        return tmp;
    }

    if(scan_direction == 0) {
        first_guess = 1;
        closest_cylinder_location = -1;
        for(i = 0; i < *queue_cnt; i++) {
            if(request_queue[i].cylinder < current_cylinder) {
                if(first_guess) {
                    closest_cylinder_location = i;
                    closest_cylinder_size = request_queue[i].cylinder;
                    first_guess = 0;
                } else if(request_queue[i].cylinder > closest_cylinder_size) {
                    closest_cylinder_location = i;
                    closest_cylinder_size = request_queue[i].cylinder;
                }
            }
        }

        if(closest_cylinder_location >= 0) {
            tmp = request_queue[closest_cylinder_location];
            for(i = closest_cylinder_location; i < *queue_cnt-1; i++) {
                request_queue[i] = request_queue[i+1];
            }
            (*queue_cnt)--;
            return tmp;
        }

        closest_cylinder_location = 0;
        closest_cylinder_size = request_queue[0].cylinder;
        for(i = 1; i < *queue_cnt; i++) {
            if(request_queue[i].cylinder < closest_cylinder_size) {
                closest_cylinder_location = i;
                closest_cylinder_size = request_queue[i].cylinder;
            }
        }
        tmp = request_queue[closest_cylinder_location];
        for(i = closest_cylinder_location; i < *queue_cnt-1; i++) {
            request_queue[i] = request_queue[i+1];
        }
        (*queue_cnt)--;
        return tmp;
    }
}

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder) {

    struct RCB NULLRCB, tmp;
    int i, first_guess, closest_cylinder_size, closest_cylinder_location, diff;

    if(*queue_cnt == 0) {
        NULLRCB.request_id = 0;
        NULLRCB.arrival_timestamp = 0;
        NULLRCB.cylinder = 0;
        NULLRCB.address = 0;
        NULLRCB.process_id = 0;

        return NULLRCB;
    }

    first_guess = 1;
    for(i = 0; i < *queue_cnt; i++) {
        if(abs(request_queue[i].cylinder - current_cylinder) > 0) {
            if(first_guess) {
                closest_cylinder_location = i;
                closest_cylinder_size = request_queue[i].cylinder;
                diff = abs(request_queue[i].cylinder - current_cylinder);
                first_guess = 0;
            } else if(abs(request_queue[i].cylinder - current_cylinder) < diff) {
                closest_cylinder_location = i;
                closest_cylinder_size = request_queue[i].cylinder;
                diff = abs(request_queue[i].cylinder - current_cylinder);
            } else if(abs(request_queue[i].cylinder - current_cylinder) == diff) {
                if(request_queue[i].arrival_timestamp < request_queue[closest_cylinder_location].arrival_timestamp) {
                    closest_cylinder_location = i;
                }
            }
        }
    }

    tmp = request_queue[closest_cylinder_location];
    for(i = closest_cylinder_location; i < *queue_cnt-1; i++) {
        request_queue[i] = request_queue[i+1];
    }
    (*queue_cnt)--;
    return tmp;
}
