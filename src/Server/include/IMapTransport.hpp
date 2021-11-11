#ifndef IMAP_TRANSPORT_H
#define IMAP_TRANSPORT_H

namespace MAP{
    class IMapTransport{
        public:
        IMapTransport();
        virtual ~IMapTransport();
        //Configure(someConfigModel)
        //Initialize (starts the server transport layer)
        //PollMessages(messageCallback(messagesBlock)))
        //SendMessage(broadcast,client,imessagePayLoad)
        //SendMessages(clientList,imessagePayLoad)
        //Close()
    };
}

#endif