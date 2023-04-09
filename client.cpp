#include <stdio.h>
#include <stdlib.h>

#include "lo/lo.h"

#include <sys/socket.h>

// #include "lo/lo_types_internal.h"
// #include "lo/lo_internal.h"

typedef struct _lo_inaddr
{
    union
    {
        struct in_addr addr;
        struct in6_addr addr6;
    } a;
    size_t size;
    char *iface;
} *lo_inaddr;

typedef struct _lo_address
{
    char *host;
    int socket;
    int ownsocket;
    char *port;
    int protocol;
    struct addrinfo *ai;
    struct addrinfo *ai_first;
    int errnum;
    const char *errstr;
    int ttl;
    struct _lo_inaddr addr;
    struct _lo_server *source_server;
    const char *source_path; /* does not need to be freed since it
                              * will always point to stack memory in
                              * dispatch_method() */
} *lo_address_yo;

void yo(lo_address_yo t)
{
    if (t->socket == -1)
    {
        printf("UDP socket not open\n");
        // t.socket = create_socket(t);
    }
    else
    {
        printf("UDP socket already open\n");

        // struct sockaddr_in server_addr;
        char server_message[2000];
        int server_struct_length = sizeof(t->addr);

        if (recvfrom(t->socket, server_message, sizeof(server_message), 0,
                     (struct sockaddr *)&t->addr, (socklen_t *)&server_struct_length) < 0)
        {
            printf("Error while receiving server's msg\n");
            return;
        }

        printf("Server's response: %s\n", server_message);
    }
}

void sendSynth(lo_address_yo t);
int main(int argc, char *argv[])
{
    lo_address_yo t = (lo_address_yo)lo_address_new(NULL, "57110");

    yo(t);

    sendSynth(t);

    lo_send(t, "/notify", "i", 1);
    yo(t);

    lo_send(t, "/status", "");
    yo(t);

    lo_send(t, "/s_new", "si", "psykick", 2000);
    yo(t);

    return 0;
}

void sendSynth(lo_address_yo t)
{
    /**
      * Create a new synth.

      Create a new synth from a named, compiled and already loaded synth definition, give it an ID, and add it to the tree of nodes.

      There are four ways to add the node to the tree as determined by the add action argument

      Controls may be set when creating the synth. The control arguments are the same as for the `n_set` command.

      If you send `/s_new` with a synth ID of -1, then the server will generate an ID for you. The server reserves all negative IDs. Since you don't know what the ID is, you cannot talk to this node directly later. So this is useful for nodes that are of finite duration and that get the control information they need from arguments and buses or messages directed to their group. In addition no notifications are sent when there are changes of state for this node, such as `/n_go`, `/n_end`, `/n_on`, `/n_off`.

      If you use a node ID of -1 for any other command, such as `/n_map`, then it refers to the most recently created node by `/s_new` (auto generated ID or not). This is how you can map  the controls of a node with an auto generated ID. In a multi-client situation, the only way you can be sure what node -1 refers to is to put the messages in a bundle.

      This message now supports array type tags (`$[` and `$]`) in the control/value component of the OSC message.  Arrayed control values are applied in the manner of n_setn (i.e., sequentially starting at the indexed or named control). See the linkGuides/NodeMessaging helpfile.

      * @param {object} args
      * - key: a control index or name
      * - value: floating point and integer arguments are interpreted
      *          as control value.
      * A symbol argument consisting of the letter 'c' or 'a' (for control or audio) followed by the bus's index.
      * @return OSC message
      */
    // export function synthNew(
    //   defName: string,
    //   nodeID = -1,
    //   addAction: number = AddActions.TAIL,
    //   targetID = 0,
    //   args: PairsType = [],
    // ): MsgType {
    //   return ["/s_new", defName, nodeID, addAction, targetID, ...flattenPairs(args)];
    // }

    // if (lo_send(t, "/s_new", "siii", "psykick", 1201,1,1005) == -1)
    if (lo_send(t, "/s_new", "si", "psykick", -1) == -1)
    {
        printf("OSC error %d: %s\n", lo_address_errno(t),
               lo_address_errstr(t));
    }
}
