#ifndef KERNELHEADER_H_
#define KERNELHEADER_H_

#include "Kernel/kernel/game/CClientKernel.h"
#include "Kernel/kernel/game/IClientKernel.h"
#include "Kernel/kernel/game/IClientKernelSink.h"

#include "Kernel/kernel/user/ClientUserManager.h"
#include "Kernel/kernel/user/IClientUserItem.h"
#include "Kernel/kernel/user/IUserManagerSink.h"

#include "Kernel/kernel/server/CServerItem.h"
#include "Kernel/kernel/server/IServerItem.h"
#include "Kernel/kernel/server/IServerItemSink.h"
#include "Kernel/kernel/server/ITableViewFrame.h"
#include "Kernel/kernel/server/TableViewFrame.h"

#include "Kernel/kernel/IChatSink.h"
#include "Kernel/kernel/IGameFrameSink.h"
#include "Kernel/kernel/IRoomFrameSink.h"
#include "Kernel/kernel/IStringMessageSink.h"

#include "Kernel/network/client_net/ClientNetSocket.h"

#include "Kernel/network/CSocketEngine.h"
#include "Kernel/network/CSocketMission.h"
#include "Kernel/network/ISocketEngine.h"
#include "Kernel/network/ISocketEngineSink.h"
#include "Kernel/network/QPCipher.h"

#include "Kernel/socket/Socket.h"

#include "Kernel/ProcessCommunicateManager.h"


#endif
