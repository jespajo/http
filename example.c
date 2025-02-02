#include "http.h"
#include "strings.h"

Response serve_hello(Request *request, Memory_context *context)
{
    string_dict *query_params = &request->query;

    char *name = *Get(query_params, "name");
    if (!name) {
        char static body[] = "What's your name?\n";
        return (Response){400, .body = body, .size = lengthof(body)};
    }

    char_array *body = get_string(context, "Hello, %s!\n", name);

    return (Response){200, .body = body->data, .size = body->count};
}

int main()
{
    u32  address = 0;       // 0.0.0.0  |Todo: Make this configurable with getaddrinfo().
    u32  port    = 30000;

    Memory_context *top_context = new_context(NULL);

    Server *server = create_server(address, port, top_context);

    add_route(server, GET, "/", &serve_hello);

    start_server(server);


    free_context(top_context);
    return 0;
}
