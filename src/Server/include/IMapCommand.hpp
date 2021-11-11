#ifndef IMAP_COMMAND_H
#define IMAP_COMMAND_H


namespace MAP{
    class IMapCommand{
        public:
        IMapCommand();
        virtual ~IMapCommand();

        //Evalute(serverContext,payLoad)
        //GetId();
    };
}

#endif