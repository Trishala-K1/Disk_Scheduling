#include "oslabs.h"

// empty_rcb
struct RCB empty_rcb = {0, 0, 0, 0, 0};

// checking_null_rcb
int check_null(struct RCB rcb) {
    if (rcb.request_id == 0 && rcb.arrival_timestamp == 0 && rcb.cylinder == 0 && rcb.address == 0 && rcb.process_id == 0) {
        return 1;
    } else {
        return 0;
    }
}

// handle_request_arrival_fcfs
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (check_null(current_request)) {
        return new_request;
    } else {
        request_queue[*queue_cnt] = new_request;
        (*queue_cnt) = (*queue_cnt) + 1;
        return current_request;
    }
}

// handle_request_completion_fcfs
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt) {
    if (*queue_cnt == 0) {
        return empty_rcb;
    }

    // Find the request with the earliest arrival_timestamp (FCFS)
    int firstArrive = 0;
    for (int i = 1; i < *queue_cnt; i++) {
        if (request_queue[i].arrival_timestamp < request_queue[firstArrive].arrival_timestamp) {
            firstArrive = i;
        }
    }

    struct RCB remove = request_queue[firstArrive];

    // Shift elements to remove the served request from the request queue
    for (int i = firstArrive; i < *queue_cnt - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }

    (*queue_cnt) = (*queue_cnt) - 1;
    return remove;
}

// handle_request_arrival_sstf
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (check_null(current_request)) {
        return new_request;
    } else {
        request_queue[*queue_cnt] = new_request;
        (*queue_cnt) = (*queue_cnt) + 1;
        return current_request;
    }
}

// handle_request_completion_sstf
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder) {
    if (*queue_cnt == 0) {
        return empty_rcb;
    }

    // Structure to hold the relative requests and their distances
    struct relative_req {
        struct RCB queue_rcb;
        int distance;
    };

    struct relative_req rcq[*queue_cnt];

    // Calculate distances and populate relative_req structure
    for (int i = 0; i < *queue_cnt; i++) {
        rcq[i].queue_rcb = request_queue[i];
        rcq[i].distance = abs(request_queue[i].cylinder - current_cylinder);
    }

    // Find the request with the smallest distance (Shortest Seek Time First)
    int closest_index = 0;
    int min_distance = rcq[0].distance;

    for (int i = 1; i < *queue_cnt; i++) {
        if (rcq[i].distance < min_distance) {
            min_distance = rcq[i].distance;
            closest_index = i;
        }
    }

    struct RCB next = rcq[closest_index].queue_rcb;

    // Remove the selected request from the request queue
    for (int i = closest_index; i < *queue_cnt - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }

    (*queue_cnt) = (*queue_cnt) - 1;
    return next;
}

// handle_request_arrival_look
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (check_null(current_request)) {
        return new_request;
    } else {
        request_queue[*queue_cnt] = new_request;
        (*queue_cnt) = (*queue_cnt) + 1;
        return current_request;
    }
}

// handle_request_completion_look
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder, int scan_direction) {
    if (*queue_cnt == 0) {
        return empty_rcb;
    }

    int early = -1;
    int early_index = 0;

    // Find the earliest request with the same cylinder as the current one
    for (int i = 0; i < *queue_cnt; i++) {
        if (request_queue[i].cylinder == current_cylinder) {
            if (early == -1 || request_queue[i].arrival_timestamp < request_queue[early_index].arrival_timestamp) {
                early = request_queue[i].arrival_timestamp;
                early_index = i;
            }
        }
    }

    if (early != -1) {
        // If an early request is found, remove it from the queue and return
        struct RCB result = request_queue[early_index];
        request_queue[early_index] = request_queue[(*queue_cnt) - 1];
        (*queue_cnt)--;
        return result;
    }

    // If there is no request with the same cylinder as the current one, find the closest request based on the scan_direction
    int closest = -1;
    int closest_index = 0;

    if (scan_direction == 1) {
        for (int i = 0; i < *queue_cnt; i++) {
            if (request_queue[i].cylinder > current_cylinder) {
                if (closest == -1 || request_queue[i].cylinder - current_cylinder < closest) {
                    closest = request_queue[i].cylinder - current_cylinder;
                    closest_index = i;
                }
            }
        }
    } else {
        for (int i = 0; i < *queue_cnt; i++) {
            if (request_queue[i].cylinder < current_cylinder) {
                if (closest == -1 || current_cylinder - request_queue[i].cylinder < closest) {
                    closest = current_cylinder - request_queue[i].cylinder;
                    closest_index = i;
                }
            }
        }
    }

    // Remove the closest request from the queue and return
    struct RCB result = request_queue[closest_index];
    request_queue[closest_index] = request_queue[(*queue_cnt) - 1];
    (*queue_cnt)--;
    return result;
}
