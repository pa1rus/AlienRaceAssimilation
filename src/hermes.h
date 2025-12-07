#ifndef HERMES_H
#define HERMES_H

#include <enet/enet.h>
#include <uuid/uuid.h>
#include <stdint.h>
#include <stdbool.h> // Include for clarity on boolean types (though C uses int/pointers for these)

/*
 * Note on C Types for Zig Exports:
 * - *c.uuid_t in Zig is uuid_t * in C (Pointer to a 16-byte array).
 * - *c_int in Zig is int * in C (Pointer to a C integer).
 * - *u8 in Zig is uint8_t * in C.
 * - *[16]u8 in Zig is char (*)[16] in C (Pointer to a 16-byte array/string).
 * - [*c]f32 in Zig is float * in C.
 */

/* Request codes */
typedef enum {
    HERMES_REQ_GET_UUID = 0,
    HERMES_REQ_ASSIGN_NAME = 1,
    // Room related requests
    HERMES_REQ_CREATE_ROOM = 2,
    HERMES_REQ_JOIN_ROOM = 3,
    HERMES_REQ_LIST_ROOMS = 4,
    HERMES_REQ_LEAVE_ROOM = 5,
    HERMES_REQ_BEGIN_RACE = 6,
    // Game related requests
    HERMES_REQ_LOADED = 7,
    HERMES_REQ_START_GAME = 8,
    HERMES_REQ_GAME_PACKET = 9,
} HermesRequest;

/* Result codes */
typedef enum {
    HERMES_OK = 0,
    HERMES_BAD_REQUEST = 1,
    HERMES_SERVER_ERROR = 2,
} HermesResult;

#ifdef __cplusplus
extern "C" {
#endif

/* Initialization & Shutdown */
void hermesInit(void);
void hermesDeinit(void);

/* Basic requests */
void hermesGetUuid(void);

/* Room management */
// room_name: Pointer to a 16-byte buffer (char array) containing the room name.
void hermesCreateRoom(uuid_t *uuid, char (*room_name)[16]); 
void hermesJoinRoom(uuid_t *client_id, uuid_t *host_id);
void hermesLeaveRoom(uuid_t *client_id, uuid_t *opponent_id);
void hermesListRooms(void);

/* Game flow */
void hermesStartGame(uuid_t *client_id);
void hermesLoaded(uuid_t *client_id);

/* Game data */
void hermesSendGameData(uuid_t *client_id, float *x, float *y, float *angle);

/* Polling */
// The names parameter is a pointer to the start of a flat buffer of room names.
void hermesPolling(
    uuid_t *uuid,             // Output: Receives the client's UUID (16 bytes)
    int *got_opponent,        // Output: Flag indicating an opponent joined (C int)
    char *names,              // Output: Flat buffer for room names (e.g., 16 rooms * 16 bytes/name)
    uint8_t *room_ammount,    // Input/Output: Number of rooms expected/received
    int *start_game,          // Output: Flag indicating game start
    int *leave_room,          // Output: Flag indicating room left
    int *begin_race,          // Output: Flag indicating race start
    float *player_x,          // Output: Array for player X coordinates
    float *player_y,          // Output: Array for player Y coordinates
    float *player_angle       // Output: Single player angle (based on Zig signature)
);

#ifdef __cplusplus
}
#endif

#endif /* HERMES_H */
