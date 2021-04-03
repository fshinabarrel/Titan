#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Titan.h>
#include <TitanMain.h>
#include "math.h"
//#include "hal_data.h"
//#include <GPT_HAL_MG.h>

struct node *head = NULL;
struct node *tail = NULL;
struct node *current = NULL;
struct node *currentPrint = NULL;
TX_BLOCK_POOL my_pool;
TX_BLOCK_POOL my_pool2;
TX_BLOCK_POOL my_pool3;
TX_BLOCK_POOL my_pool4;
TX_BYTE_POOL USB_Byte_Pool;
TX_BYTE_POOL USB_Byte_PoolB;

void initMotors()
{
    motorInitX ();

    motorBlockY->controlCode = 'y';
    motorBlockY->dirPin = IOPORT_PORT_06_PIN_07;
    motorBlockY->stepPin = IOPORT_PORT_03_PIN_04;
    motorBlockY->limit0Pin = IOPORT_PORT_05_PIN_12;
    genericMotorInit (motorBlockY);

    motorBlockA->controlCode = 'a';
    motorBlockA->dirPin = IOPORT_PORT_06_PIN_07;
    motorBlockA->stepPin = IOPORT_PORT_03_PIN_04;
    motorBlockA->limit0Pin = IOPORT_PORT_05_PIN_12;
    genericMotorInit (motorBlockA);

    motorBlockZ->controlCode = 'z';
    motorBlockZ->dirPin = IOPORT_PORT_06_PIN_01;
    motorBlockZ->stepPin = IOPORT_PORT_01_PIN_11;
    motorBlockZ->limit0Pin = IOPORT_PORT_00_PIN_00;
    genericMotorInit (motorBlockZ);

    motorBlockB->controlCode = 'b';
    motorBlockB->dirPin = IOPORT_PORT_06_PIN_01;
    motorBlockB->stepPin = IOPORT_PORT_01_PIN_11;
    motorBlockB->limit0Pin = IOPORT_PORT_00_PIN_00;
    genericMotorInit (motorBlockB);

    motorBlockC->controlCode = 'c';
    motorBlockC->dirPin = IOPORT_PORT_06_PIN_01;
    motorBlockC->stepPin = IOPORT_PORT_01_PIN_11;
    motorBlockC->limit0Pin = IOPORT_PORT_00_PIN_00;
    genericMotorInit (motorBlockC);

    motorBlockD->controlCode = 'd';
    motorBlockD->dirPin = IOPORT_PORT_06_PIN_01;
    motorBlockD->stepPin = IOPORT_PORT_01_PIN_11;
    motorBlockD->limit0Pin = IOPORT_PORT_00_PIN_00;
    genericMotorInit (motorBlockD);

    motorInitT ();

    machineGlobalsBlock->motorsInit = 1;
}

void initTools()
{
    toolInitA ();
}

///Returns the length of the message buffer
int length()
{
    int length = 0;

    //if list is empty
    if (head == NULL)
    {
        return 0;
    }

    current = head->next;

    while (current != head)
    {
        length++;
        current = current->next;
    }

    return length;
}

////insert link at the first location
//void insertFirst(int key, char content[50]) {
//
//   //create a link
//   struct node *link = (struct node*) malloc(sizeof(struct node));
//   link->key = key;
//   link->content = content;
//
//   if (isEmpty()) {
//      head = link;
//      head->next = head;
//   } else {
//      //point it to old first node
//      link->next = head;
//
//      //point first to new first node
//      head = link;
//   }
//}

void initBuff()
{
    UINT status;
    unsigned char *memory_ptr;

    status = tx_block_pool_create(&my_pool, "linkbuff", sizeof(struct node), (VOID *) 0x20020000, 100000);

    status = tx_block_allocate (&my_pool, (VOID **) &memory_ptr, TX_NO_WAIT);

    if (status != TX_SUCCESS)
        printf ("Buffer init failed.");

    char tmp[10] = "Helix";
    //create a link
    head = insertLink (tmp);
    //setup pointers
    tail = head;
    current = head;
    currentPrint = head;

    head->prev = NULL;
    head->next = NULL;
    tail->prev = NULL;
    tail->next = NULL;

    memcpy (head->content, tmp, 6);
}

///Motor Block initialization process.
void initMotorBlocks()
{
    UINT status;
    unsigned char *memory_ptr;

    status = tx_block_pool_create(&my_pool2, "cntrlblocks", sizeof(struct motorController), (VOID *) 0x20040000, 10000);

    status = tx_block_allocate (&my_pool2, (VOID **) &memory_ptr, TX_NO_WAIT);
    motorBlockX = (struct motorController *) memory_ptr;
    machineGlobalsBlock->controllerBlocks[0] = motorBlockX;
    motorBlockX->init = 1;

    status = tx_block_allocate (&my_pool2, (VOID **) &memory_ptr, TX_NO_WAIT);
    motorBlockY = (struct motorController *) memory_ptr;
    machineGlobalsBlock->controllerBlocks[1] = motorBlockY;
    motorBlockY->init = 1;

    status = tx_block_allocate (&my_pool2, (VOID **) &memory_ptr, TX_NO_WAIT);
    motorBlockA = (struct motorController *) memory_ptr;
    motorBlockA->init = 1;

    status = tx_block_allocate (&my_pool2, (VOID **) &memory_ptr, TX_NO_WAIT);
    motorBlockZ = (struct motorController *) memory_ptr;
    machineGlobalsBlock->controllerBlocks[2] = motorBlockZ;
    motorBlockZ->init = 1;

    status = tx_block_allocate (&my_pool2, (VOID **) &memory_ptr, TX_NO_WAIT);
    motorBlockB = (struct motorController *) memory_ptr;
    motorBlockB->init = 1;

    status = tx_block_allocate (&my_pool2, (VOID **) &memory_ptr, TX_NO_WAIT);
    motorBlockC = (struct motorController *) memory_ptr;
    machineGlobalsBlock->controllerBlocks[2] = motorBlockZ;
    motorBlockC->init = 1;

    status = tx_block_allocate (&my_pool2, (VOID **) &memory_ptr, TX_NO_WAIT);
    motorBlockD = (struct motorController *) memory_ptr;
    machineGlobalsBlock->controllerBlocks[2] = motorBlockZ;
    motorBlockD->init = 1;

    status = tx_block_allocate (&my_pool2, (VOID **) &memory_ptr, TX_NO_WAIT);
    motorBlockT = (struct motorController *) memory_ptr;
    motorBlockT->init = 1;

    machineGlobalsBlock->numOfControllers = 3;

//    status = tx_block_allocate (&my_pool2, (VOID **) &memory_ptr, TX_NO_WAIT);
//    motorBlock3 = (struct motorController *) memory_ptr;
//    motorBlock3->init = 1;
}

///Tool block initialization process.
void initToolBlocks()
{
    UINT status;
    unsigned char *memory_ptr;

    status = tx_block_pool_create(&my_pool4, "toolblocks", sizeof(struct toolBlock), (VOID *) 0x20045000, 1000);

    status = tx_block_allocate (&my_pool4, (VOID **) &memory_ptr, TX_NO_WAIT);
    toolBlockA = (struct toolBlock *) memory_ptr;
}

void initGlobalsBlock()
{
    //init memory pool and block for retaining machine status information
    UINT status;
    ssp_err_t err;
    unsigned char *memory_ptr;

    status = tx_block_pool_create(&my_pool3, "globals", sizeof(struct machineGlobals), (VOID *) 0x20050000, 2000);

    status = tx_block_allocate (&my_pool3, (VOID **) &memory_ptr, TX_NO_WAIT);
    machineGlobalsBlock = (struct machineGlobals *) memory_ptr;
    machineGlobalsBlock->iniInit = 0;
    machineGlobalsBlock->receivingMsg = 0;
    machineGlobalsBlock->calibRunning = 0;
    machineGlobalsBlock->targetSpeed = DEFAULTSPEED;
    ///Default to relative positioning.
    machineGlobalsBlock->relativePositioningEN = 0;
    machineGlobalsBlock->USBPlugIn = 0;
    machineGlobalsBlock->USBBufferOpen = 0;
    machineGlobalsBlock->USBINIReady = 0;
    machineGlobalsBlock->USBBufferHasData = 0;
    machineGlobalsBlock->USBFileIndex = 0;
    machineGlobalsBlock->local_bufferIndex = 0;
    machineGlobalsBlock->motorsInit = 0;
    machineGlobalsBlock->linkedListNodeCount = 0;
    machineGlobalsBlock->rebuildLinkedList = 0;
    machineGlobalsBlock->UDPTimeout = 0;
    machineGlobalsBlock->UDPRX = 0;
    machineGlobalsBlock->echoWaitStart = 0;
    machineGlobalsBlock->UDPFlowControl = 0;
    machineGlobalsBlock->motorFreqSet = 0xFF;
    machineGlobalsBlock->getUpdate = 0;
    machineGlobalsBlock->nextIP = IP_ADDRESS(192, 168, 10, 183);
    machineGlobalsBlock->UDPRxIP = 0;

    machineGlobalsBlock->controllerIndex = 0;

    ///Relay init
    err = g_ioport.p_api->pinWrite (IOPORT_PORT_02_PIN_03, IOPORT_LEVEL_HIGH);
    err = g_ioport.p_api->pinWrite (IOPORT_PORT_02_PIN_03, IOPORT_LEVEL_HIGH);
    err = g_ioport.p_api->pinWrite (IOPORT_PORT_02_PIN_03, IOPORT_LEVEL_HIGH);
    err = g_ioport.p_api->pinWrite (IOPORT_PORT_02_PIN_03, IOPORT_LEVEL_HIGH);

    machineGlobalsBlock->globalsInit = 1;

}

char *initUSBBuffer_Pool(UINT size)
{
    unsigned char *memory_ptr;
    UINT status;
    ///Create pool
    status = tx_byte_pool_create(&USB_Byte_Pool, "usb_byte_pool", (VOID *) 0x20060000, size + 128);

    ///Allocate bytes
    status = tx_byte_allocate (&USB_Byte_Pool, (VOID **) &memory_ptr, size, TX_NO_WAIT);
    if (status != TX_SUCCESS)
    {
        printf ("USB Buffer Init Failed.");
    }
    memset (memory_ptr, 0, size);
    return memory_ptr;
}

char *initUSBBuffer_PoolB(UINT size)
{
    unsigned char *memory_ptr;
    UINT status;
    ///Create pool
    status = tx_byte_pool_create(&USB_Byte_PoolB, "usb_byte_pool", (VOID *) 0x20070000, size + 64);

    ///Allocate bytes
    status = tx_byte_allocate (&USB_Byte_PoolB, (VOID **) &memory_ptr, size, TX_NO_WAIT);
    if (status != TX_SUCCESS)
    {
        printf ("USB Buffer Init Failed.");
    }
    memset (memory_ptr, 0, size);
    return memory_ptr;
}

//insert link at end of buffer
struct node *insertLink(char *content)
{
    UINT status;
    unsigned char *memory_ptr;

    //create a link
    status = tx_block_allocate (&my_pool, (VOID **) &memory_ptr, TX_NO_WAIT);
    memset (memory_ptr, 0, sizeof(struct node));
    struct node *link = (struct node *) memory_ptr;

//    memcpy(link->content, content, NXD_MQTT_MAX_MESSAGE_LENGTH);
//    memcpy (link->content, content, strlen (content));
    memcpy (link->content, content, WIFI_PACKET_SIZE);

    if (head == NULL && tail == NULL)
    {
        ///Starting new list
        link->prev = NULL;
        link->next = NULL;
        head = link;
        tail = link;
        current = link;
        currentPrint = link;
        ///Raise the event flag to indicate a linked list node is available for the Main thread.
        status = tx_event_flags_set (&g_linked_list_flags, 1, TX_OR);
    }
    else
    {
        link->prev = tail;
        link->next = NULL;
        tail->next = link;
        tail = link;
    }

    machineGlobalsBlock->linkedListNodeCount++;
    if (DEBUGGER)
        if (!link)
            printf ("Build Link Fail.");

    return link;
}

/**Releases the block memory of a link, and returns the next link,
 if possible, otherwise a NULL*/
struct node *removeLink(struct node *link)
{
    struct node *tmp;
    UINT status;

//    if(DEBUG) printf("\nRemoving:%s", link->content);

    if (link->prev != NULL)
    {
        ///Not the head.
        if (link->next != NULL)
        {
            ///More link(s) follow.
            link->prev->next = link->next;
            link->next->prev = link->prev;

            tmp = link->next;

        }
        else
        {
            ///No more links. This is the tail.
            link->prev->next = NULL;

            tmp = link->prev;
        }

    }
    else
    {
        ///Link is the head.
        if (link->next != NULL)
        {
            ///Not the only link.
            link->next->prev = NULL;
            head = link->next;
            tmp = link->next;
        }
        else
        {
            ///This is the only link.
            ///Release memory block and return NULL.
            head = NULL;
            tail = NULL;
            tmp = NULL;
            ///Clear the linked list event flag.
            status = tx_event_flags_set (&g_linked_list_flags, 0, TX_AND);
        }
    }

    status = tx_block_release (link);
    machineGlobalsBlock->linkedListNodeCount--;

    return tmp;
}

//display the list
void printList()
{
    printf ("\nPrinting List:...\n");
    current = head;

    while (current->next != NULL)
    {
        printf ("\n%s", current->content);
        current = current->next;
    }
    printf ("\n%s", current->content);
}

void commandHandler(struct instruction *data)
{
    if (strcmp (data->cmd, "G01") == 0)
    {
//        if(DEBUG) printf ("G01...");

        ///Run the G01 handler
        G01 (data);
    }
    else if (strcmp (data->cmd, "G10") == 0)
    {
        if (strchr (data->cmdString, 'a') || strchr (data->cmdString, 'A'))
        {
            UDPSetTemperature ('a', data->a);
        }
    }
    else if (strcmp (data->cmd, "G28") == 0)
    {
        if (strchr (data->cmdString, 'x') || strchr (data->cmdString, 'X'))
        {
            UDPHomeMotor (motorBlockX);
        }
        if (strchr (data->cmdString, 'y') || strchr (data->cmdString, 'Y'))
        {
            UDPHomeMotor (motorBlockY);
            UDPHomeMotor (motorBlockA);
        }
        if (strchr (data->cmdString, 'z') || strchr (data->cmdString, 'Z'))
        {
            UDPHomeMotor (motorBlockZ);
        }
    }
    else if (strcmp (data->cmd, "G54") == 0)
    {
        if (DEBUGGER)
            printf ("G54...");
        //set axes offsets
        UDPZeroAxes ();
    }
    else if (strcmp (data->cmd, "G90") == 0)
    {
        if (DEBUGGER)
            printf ("G90...");
        machineGlobalsBlock->relativePositioningEN = 0;
    }
    else if (strcmp (data->cmd, "G91") == 0)
    {
        if (DEBUGGER)
            printf ("G91...");
        machineGlobalsBlock->relativePositioningEN = 1;
    }
    else if (strcmp (data->cmd, "MSS") == 0)
    {
        if (DEBUGGER)
            printf ("MSS...");
        //set motor step sizes

        if (strchr (data->cmdString, 'x') || strchr (data->cmdString, 'X'))
        {
            if (data->x != ~0)
            {

                motorBlockX->stepSize = data->x;
                ///Get Forward Logic Level
                if (strchr (data->cmdString, 'H'))
                {
                    motorBlockX->fwdDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    motorBlockX->fwdDir = IOPORT_LEVEL_LOW;
                }
                ///Get Home direction
                if (strchr (data->cmdString, 'U'))
                {
                    motorBlockX->defaultDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    motorBlockX->defaultDir = IOPORT_LEVEL_LOW;
                }

                if (data->f != ~0)
                {
                    motorBlockX->homeSpeed = data->f;
                }

                UDPSendINI (motorBlockX);
            }

        }
        if (strchr (data->cmdString, 'y') || strchr (data->cmdString, 'Y'))
        {
            if (data->y != ~0)
            {
                motorBlockY->stepSize = data->y;
                ///Get Forward Logic Level
                if (strchr (data->cmdString, 'H'))
                {
                    motorBlockY->fwdDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    motorBlockY->fwdDir = IOPORT_LEVEL_LOW;
                }
                ///Get Home direction
                if (strchr (data->cmdString, 'U'))
                {
                    motorBlockY->defaultDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    motorBlockY->defaultDir = IOPORT_LEVEL_LOW;
                }

                if (data->f != ~0)
                {
                    motorBlockY->homeSpeed = data->f;
                }

                UDPSendINI (motorBlockY);
            }

        }
        if (strchr (data->cmdString, 'a') || strchr (data->cmdString, 'A'))
        {
            if (data->a != ~0)
            {
                motorBlockA->stepSize = data->a;
                ///Get Forward Logic Level
                if (strchr (data->cmdString, 'H'))
                {
                    motorBlockA->fwdDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    motorBlockA->fwdDir = IOPORT_LEVEL_LOW;
                }
                ///Get Home direction
                if (strchr (data->cmdString, 'U'))
                {
                    motorBlockA->defaultDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    motorBlockA->defaultDir = IOPORT_LEVEL_LOW;
                }

                if (data->f != ~0)
                {
                    motorBlockA->homeSpeed = data->f;
                }

                UDPSendINI (motorBlockA);
            }

        }
        if (strchr (data->cmdString, 'z') || strchr (data->cmdString, 'Z'))
        {
            if (data->z != ~0)
            {
                motorBlockZ->stepSize = data->z;
                ///Get Forward Logic Level
                if (strchr (data->cmdString, 'H'))
                {
                    motorBlockZ->fwdDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    motorBlockZ->fwdDir = IOPORT_LEVEL_LOW;
                }
                ///Get Home direction
                if (strchr (data->cmdString, 'U'))
                {
                    motorBlockZ->defaultDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    motorBlockZ->defaultDir = IOPORT_LEVEL_LOW;
                }

                if (data->f != ~0)
                {
                    motorBlockZ->homeSpeed = data->f;
                }

                UDPSendINI (motorBlockZ);
            }

        }
        if (strchr (data->cmdString, 'b') || strchr (data->cmdString, 'B'))
        {
            if (data->b != ~0)
            {
                motorBlockB->stepSize = data->b;
                ///Get Forward Logic Level
                if (strchr (data->cmdString, 'H'))
                {
                    motorBlockB->fwdDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    motorBlockB->fwdDir = IOPORT_LEVEL_LOW;
                }
                ///Get Home direction
                if (strchr (data->cmdString, 'U'))
                {
                    motorBlockB->defaultDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    motorBlockB->defaultDir = IOPORT_LEVEL_LOW;
                }

                if (data->f != ~0)
                {
                    motorBlockB->homeSpeed = data->f;
                }

                UDPSendINI (motorBlockB);
            }

        }
        if (strchr (data->cmdString, 'c') || strchr (data->cmdString, 'C'))
        {
            if (data->c != ~0)
            {
                motorBlockC->stepSize = data->c;
                ///Get Forward Logic Level
                if (strchr (data->cmdString, 'H'))
                {
                    motorBlockC->fwdDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    motorBlockC->fwdDir = IOPORT_LEVEL_LOW;
                }
                ///Get Home direction
                if (strchr (data->cmdString, 'U'))
                {
                    motorBlockC->defaultDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    motorBlockC->defaultDir = IOPORT_LEVEL_LOW;
                }

                if (data->f != ~0)
                {
                    motorBlockC->homeSpeed = data->f;
                }

                UDPSendINI (motorBlockC);
            }

        }
        if (strchr (data->cmdString, 'd') || strchr (data->cmdString, 'D'))
        {
            if (data->d != ~0)
            {
                motorBlockD->stepSize = data->d;
                ///Get Forward Logic Level
                if (strchr (data->cmdString, 'H'))
                {
                    motorBlockD->fwdDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    motorBlockD->fwdDir = IOPORT_LEVEL_LOW;
                }
                ///Get Home direction
                if (strchr (data->cmdString, 'U'))
                {
                    motorBlockD->defaultDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    motorBlockD->defaultDir = IOPORT_LEVEL_LOW;
                }

                if (data->f != ~0)
                {
                    motorBlockD->homeSpeed = data->f;
                }

                UDPSendINI (motorBlockD);
            }

        }
        if (strchr (data->cmdString, 't') || strchr (data->cmdString, 'T'))
        {
            if (data->a != ~0)
            {

                toolBlockA->motorBlock->stepSize = data->a;
                ///Get Forward Logic Level
                if (strchr (data->cmdString, 'H'))
                {
                    toolBlockA->motorBlock->fwdDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    toolBlockA->motorBlock->fwdDir = IOPORT_LEVEL_LOW;
                }
                ///Get Home direction
                if (strchr (data->cmdString, 'U'))
                {
                    toolBlockA->motorBlock->defaultDir = IOPORT_LEVEL_HIGH;
                }
                else
                {
                    toolBlockA->motorBlock->defaultDir = IOPORT_LEVEL_LOW;
                }

                if (data->f != ~0)
                {
                    toolBlockA->motorBlock->homeSpeed = data->f;
                }

                UDPSendINI (motorBlockA);
            }

        }
//        printf ("\nInitiating INI save...");
        saveINI ();
    }
    else if (strcmp (data->cmd, "RLY") == 0)
    {
        ssp_err_t err;

        if (DEBUGGER)
            printf ("\nRelay instruction...");

        if (strchr (data->cmdString, 'a') || strchr (data->cmdString, 'A'))
        {
            if (data->a != ~0)
            {
                if (data->a == 0)
                {
                    ///Set relay LOW
                    UDPSetRelay ('x', '0', 'l');
                }
                else
                {
                    ///Set relay HIGH
                    UDPSetRelay ('x', '0', 'h');
                }
            }
        }
        if (strchr (data->cmdString, 'b') || strchr (data->cmdString, 'B'))
        {
            if (data->b != ~0)
            {
                if (data->b == 0)
                {
                    ///Set relay LOW
                    UDPSetRelay ('x', '1', 'l');
                }
                else
                {
                    ///Set relay HIGH
                    UDPSetRelay ('x', '1', 'h');
                }
            }
        }
        if (strchr (data->cmdString, 'c') || strchr (data->cmdString, 'C'))
        {
            if (data->c != ~0)
            {
                if (data->c == 0)
                {
                    ///Set relay LOW
                    UDPSetRelay ('x', '2', 'l');
                }
                else
                {
                    ///Set relay HIGH
                    UDPSetRelay ('x', '2', 'h');
                }
            }
        }
        if (strchr (data->cmdString, 'd') || strchr (data->cmdString, 'D'))
        {
            if (data->d != ~0)
            {
                if (data->d == 0)
                {
                    ///Set relay LOW
                    UDPSetRelay ('x', '3', 'l');
                }
                else
                {
                    ///Set relay HIGH
                    UDPSetRelay ('x', '3', 'h');
                }
            }
        }
    }
    else if (strcmp (data->cmd, "CAL") == 0)
    {

        if (DEBUGGER)
            printf ("\nCAL Run Start...");

        calCmdHandler (data);
    }
    else if (strcmp (data->cmd, "M03") == 0)
    {

        if (DEBUGGER)
            printf ("\nMotor RUN FWD...");

        if (strchr (data->cmdString, 'x') || strchr (data->cmdString, 'X'))
        {
            if (data->x != ~0)
            {
                UDPSetMotorDir (motorBlockX, 1);
                UDPRunMotorFrequency (motorBlockX, data->x);

//                motorBlockX->targetDir = motorBlockX->fwdDir;
            }

        }
        if (strchr (data->cmdString, 'y') || strchr (data->cmdString, 'Y'))
        {
            if (data->y != ~0)
            {
                UDPSetMotorDir (motorBlockY, 1);
                UDPRunMotorFrequency (motorBlockY, data->y);
            }

        }
        if (strchr (data->cmdString, 'a') || strchr (data->cmdString, 'A'))
        {
            if (data->a != ~0)
            {
                UDPSetMotorDir (motorBlockA, 1);
                UDPRunMotorFrequency (motorBlockA, data->a);
            }

        }
        if (strchr (data->cmdString, 'z') || strchr (data->cmdString, 'Z'))
        {
            if (data->z != ~0)
            {
                UDPSetMotorDir (motorBlockZ, 1);
                UDPRunMotorFrequency (motorBlockZ, data->z);
            }

        }
        if (strchr (data->cmdString, 'b') || strchr (data->cmdString, 'B'))
        {
            if (data->b != ~0)
            {
                UDPSetMotorDir (motorBlockB, 1);
                UDPRunMotorFrequency (motorBlockB, data->b);
            }

        }
        if (strchr (data->cmdString, 'c') || strchr (data->cmdString, 'C'))
        {
            if (data->c != ~0)
            {
                UDPSetMotorDir (motorBlockC, 1);
                UDPRunMotorFrequency (motorBlockC, data->c);
            }

        }
        if (strchr (data->cmdString, 'd') || strchr (data->cmdString, 'D'))
        {
            if (data->d != ~0)
            {
                UDPSetMotorDir (motorBlockD, 1);
                UDPRunMotorFrequency (motorBlockD, data->d);
            }

        }
        if (strchr (data->cmdString, 't') || strchr (data->cmdString, 'T'))
        {
//            if (data->a != ~0)
//            {
//                UDPSetMotorDir (motorBlockA, 1);
//                UDPRunMotorFrequency (motorBlockA, data->a);
//            }

        }
    }
    else if (strcmp (data->cmd, "M04") == 0)
    {

        if (DEBUGGER)
            printf ("\nMotor RUN REV...");

        if (strchr (data->cmdString, 'x') || strchr (data->cmdString, 'X'))
        {
            if (data->x != ~0)
            {
                UDPSetMotorDir (motorBlockX, 0);
                UDPRunMotorFrequency (motorBlockX, data->x);
            }

        }
        if (strchr (data->cmdString, 'y') || strchr (data->cmdString, 'Y'))
        {
            if (data->y != ~0)
            {
                UDPSetMotorDir (motorBlockY, 0);
                UDPRunMotorFrequency (motorBlockY, data->y);
            }

        }
        if (strchr (data->cmdString, 'z') || strchr (data->cmdString, 'Z'))
        {
            if (data->z != ~0)
            {
                UDPSetMotorDir (motorBlockZ, 0);
                UDPRunMotorFrequency (motorBlockZ, data->z);
            }

        }
        if (strchr (data->cmdString, 'a') || strchr (data->cmdString, 'A'))
        {
            if (data->a != ~0)
            {
                UDPSetMotorDir (motorBlockA, 0);
                UDPRunMotorFrequency (motorBlockA, data->a);
            }

        }
        if (strchr (data->cmdString, 'b') || strchr (data->cmdString, 'B'))
        {
            if (data->b != ~0)
            {
                UDPSetMotorDir (motorBlockB, 0);
                UDPRunMotorFrequency (motorBlockB, data->b);
            }

        }
        if (strchr (data->cmdString, 'c') || strchr (data->cmdString, 'C'))
        {
            if (data->c != ~0)
            {
                UDPSetMotorDir (motorBlockC, 0);
                UDPRunMotorFrequency (motorBlockC, data->c);
            }

        }
        if (strchr (data->cmdString, 'd') || strchr (data->cmdString, 'D'))
        {
            if (data->d != ~0)
            {
                UDPSetMotorDir (motorBlockD, 0);
                UDPRunMotorFrequency (motorBlockD, data->d);
            }

        }
    }
    else if (strcmp (data->cmd, "AUL") == 0)
    {
        ///Auto-level
        if (DEBUGGER)
            printf ("\nAuto-level...");

        autoBuildPlateLevel ();

    }
    else
    {
        if (DEBUGGER)
            printf ("\nInvalid instruction - Main");
    }
}

///Currently being designed for a conductive touch probe method.
/// Possible impediments to this include residual material on the nozzle. This should be
/// alleviated by the nozzle being brought to working temperature, but there remains some possibility
/// of caramelized or charred material being left behind to impede the flow of current. Further investigation is
/// required.
void autoBuildPlateLevel()
{
    ///The auto-level procedure requires a conductive or pressure-sensitive tool.
    /// The auto-level procedure is as follows:
    /// 1)Raise all four Z axes until the build plate touches the probe.

    struct instruction data;

    ///First, zero the axes and lower the build plate by 10mm. This is performed to ensure that the nozzle can clear the build plate.
    UDPZeroAxes ();

    data.x = ~0;
    data.y = ~0;
    data.z = 25;
    data.a = ~0;
    data.f = 1000;
    G01 (&data);

    ///Second, home the X and Y axes.
    UDPHomeMotor (motorBlockX);
    UDPHomeMotor (motorBlockY);
    UDPHomeMotor (motorBlockA);

    ///Third, send the nozzle to the center (-75, -75).
    data.x = -75;
    data.y = -75;
    data.z = ~0;
    data.a = ~0;
    data.f = 1000;
    G01 (&data);

    UDPHomeMotor (motorBlockZ);
    UDPHomeMotor (motorBlockB);
    UDPHomeMotor (motorBlockC);
    UDPHomeMotor (motorBlockD);
}

void calCmdHandler(struct instruction *data)
{
    double targetPosSteps;
    ///Only one axis may be calibrated at a time.
    /// Check the command string for an x, y, or z. Run the calibration
    /// process for the axis found, if any.
    if (data->f != ~0) ///The calibration process requires a frequency to be specified.
    {
        int dir;
        double targetPosSteps;
        if (strchr (data->cmdString, 'x') || strchr (data->cmdString, 'X'))
        {
            ///The X-axis is in the command string. Is there a valid target position?
            if (data->x != ~0)
            {
                ///Is the calibration in the forward or reverse direction?
                if (strchr (data->cmdString, 'U'))
                {
                    ///Forward
                    dir = 1;
                    targetPosSteps = data->x;
                }
                else
                {
                    dir = 0;
                    targetPosSteps = data->x;
                    targetPosSteps = -targetPosSteps;
                }

                UDPCalibrateMotor (motorBlockX, data->f, targetPosSteps, dir);
                ///There is a valid target position. Run the calibration process for this axis.
//                calRoutine (data, motorBlockX, (long int) data->x, (int) data->f);
            }
        }
        else if (strchr (data->cmdString, 'y') || strchr (data->cmdString, 'Y'))
        {
            ///The Y-axis is in the command string. Is there a valid target position?
            if (data->y != ~0)
            {
                ///Is the calibration in the forward or reverse direction?
                if (strchr (data->cmdString, 'U'))
                {
                    ///Forward
                    dir = 1;
                    targetPosSteps = data->y;
                }
                else
                {
                    dir = 0;
                    targetPosSteps = data->y;
                    targetPosSteps = -targetPosSteps;
                }

                UDPCalibrateMotor (motorBlockY, data->f, targetPosSteps, dir);
            }
        }
        else if (strchr (data->cmdString, 'z') || strchr (data->cmdString, 'Z'))
        {
            ///The X-axis is in the command string. Is there a valid target position?
            if (data->z != ~0)
            {
                ///Is the calibration in the forward or reverse direction?
                if (strchr (data->cmdString, 'U'))
                {
                    ///Forward
                    dir = 1;
                    targetPosSteps = data->z;
                }
                else
                {
                    dir = 0;
                    targetPosSteps = data->z;
                    targetPosSteps = -targetPosSteps;
                }

                UDPCalibrateMotor (motorBlockZ, data->f, targetPosSteps, dir);
            }
        }
        else if (strchr (data->cmdString, 'a') || strchr (data->cmdString, 'A'))
        {
            ///The A-axis is in the command string. Is there a valid target position?
            if (data->a != ~0)
            {
                ///Is the calibration in the forward or reverse direction?
                if (strchr (data->cmdString, 'U'))
                {
                    ///Forward
                    dir = 1;
                    targetPosSteps = data->a;
                }
                else
                {
                    dir = 0;
                    targetPosSteps = data->a;
                    targetPosSteps = -targetPosSteps;
                }

                UDPCalibrateMotor (motorBlockA, data->f, targetPosSteps, dir);
            }
        }
    }
}

///This function is responsible for monitoring motor activity during a
/// calibration routine.
void calRoutine(struct instruction *data, struct motorController *motorBlock, long int targetPosSteps, int targetFreq)
{
    long int tmpTargetPosSteps;
    tmpTargetPosSteps = motorBlock->posSteps;

    ///Reset the motor position to zero
    motorBlock->posSteps = 0;
    motorBlock->pos = 0;

    ///Set forward logic level;
    if (strchr (data->cmdString, 'H'))
    {
        motorBlock->fwdDir = IOPORT_LEVEL_HIGH;
    }
    else
    {
        motorBlock->fwdDir = IOPORT_LEVEL_LOW;
    }

    ///Is the calibration in the forward or reverse direction?
    if (strchr (data->cmdString, 'U'))
    {
        ///Forward
        tmpTargetPosSteps = motorBlock->posSteps + targetPosSteps;
    }
    else
    {
        tmpTargetPosSteps = motorBlock->posSteps - targetPosSteps;
    }
    ///Set the motor to be adjusted according to frequency.
    motorBlock->freqSet = 1;

    ///Set the target direction
    if (strchr (data->cmdString, 'U'))
    {
        ///Forward
        motorBlock->targetDir = motorBlock->fwdDir;
    }
    else
    {
        if (motorBlock->fwdDir == IOPORT_LEVEL_HIGH)
        {
            motorBlock->targetDir = IOPORT_LEVEL_LOW;
        }
        else
        {
            motorBlock->targetDir = IOPORT_LEVEL_HIGH;
        }
    }

    ///The motor is ready to begin the calibration run.
    /// Set the target frequency to begin.
    motorBlock->targetFreq = targetFreq;

    ///At this point the motor should begin moving.
    ///Now, we must monitor the position of the axis and stop its movement
    /// as soon as it gets within range of the target position.
    if (strchr (data->cmdString, 'U'))
    {
        while (motorBlock->posSteps < tmpTargetPosSteps)
        {
            tx_thread_sleep (1);
        }
    }
    else
    {
        while (motorBlock->posSteps > tmpTargetPosSteps)
        {
            tx_thread_sleep (1);
        }
    }
    ///Axis has reached target position. Stop it immediately.
    motorBlock->targetFreq = 0;

}

/**Can be used to return the percent error between a target and current float.
 Useful when determing the percent error of positioning.*/
double percentError(double target, double actual)
{
    double diff = fabs (actual - target);
    double error = fabs (diff / target);
    error *= 100.0;
    return error;
}

char isInRange(char in)
{
    if ((in > 44 && in < 58) || (in > 64 && in < 91) || (in > 96 && in < 123))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

///setupMode handles the Setup Mode process, which associates IP addresses with axes.
void setupMode()
{
    UINT status;
    ULONG event;
    printf ("\nSetup Start.");
    status = tx_event_flags_set (&g_udp_echo_received, 1, TX_AND);

    ///Reset the index to 0.
    machineGlobalsBlock->controllerIndex = 0;

    ///Wait until the index reaches the end.
    /// The UDP receive function will handle assigning IP addresses and incrementing the index.
    status = tx_event_flags_get (&g_setup_mode_complete, 1, TX_AND_CLEAR, &event, NX_WAIT_FOREVER);
    printf ("\nSetup Complete1.");
}

void processReceivedMsg(char *message_buffer)
{
//    if (machineGlobalsBlock->receivingMsg != 1)
//        machineGlobalsBlock->receivingMsg = 1;

    ///Check for STOP message
    if (message_buffer[0] == 'M' && message_buffer[1] == '0' && message_buffer[2] == '0')
    {
        printf ("\nMotor Stop...");
        ///Stop each motor in message
        if (strchr (message_buffer, 'x') || strchr (message_buffer, 'X'))
        {
            stopMotor (motorBlockX);
        }
        if (strchr (message_buffer, 'y') || strchr (message_buffer, 'Y'))
        {
            stopMotor (motorBlockY);
        }
        if (strchr (message_buffer, 'z') || strchr (message_buffer, 'Z'))
        {
            stopMotor (motorBlockZ);
        }
        if (strchr (message_buffer, 'a') || strchr (message_buffer, 'A'))
        {
            stopMotor (motorBlockA);
        }
        if (strchr (message_buffer, 'b') || strchr (message_buffer, 'B'))
        {
            stopMotor (motorBlockB);
        }
        if (strchr (message_buffer, 'c') || strchr (message_buffer, 'C'))
        {
            stopMotor (motorBlockC);
        }
        if (strchr (message_buffer, 'd') || strchr (message_buffer, 'D'))
        {
            stopMotor (motorBlockD);
        }
    }
//    ///Lower flag if terminating message found
//    else if (message_buffer[0] == 'X' && message_buffer[1] == 'X' && message_buffer[2] == 'X')
//    {
//        if (machineGlobalsBlock->local_bufferIndex > 0)
//        {
//            USB_Buffer_Transfer ();
//        }
//        machineGlobalsBlock->receivingMsg = 0;
//    }
    else if (message_buffer[0] == 'S' && message_buffer[1] == 'T' && message_buffer[2] == 'P')
    {

    }
    else if (message_buffer[0] == 'H' && message_buffer[1] == 'L' && message_buffer[2] == 'X')
    {
        ///Do nothing for keep alive msg
    }
    else
    {
//        UINT tmp_length;
        ///Add received message to linked list
        insertLink (message_buffer);
//        if (machineGlobalsBlock->USBBufferOpen)
//        {
//            //                    tmp_length = strlen(message_buffer);
//            tmp_length = 49;
//            machineGlobalsBlock->local_buffer[machineGlobalsBlock->local_bufferIndex] = '\n';
//            memcpy ((machineGlobalsBlock->local_buffer + machineGlobalsBlock->local_bufferIndex + 1), message_buffer,
//                    tmp_length);
//            machineGlobalsBlock->local_bufferIndex += (tmp_length + 1);
//            if (machineGlobalsBlock->local_bufferIndex > 1000)
//            {
//                USB_Buffer_Transfer ();
//            }
//            //                    USB_Write_Buffer (message_buffer, 3);
//            //                    tx_thread_sleep (100);
//        }
//        else
//        {
//
//        }
    }
}

///This function calculates a velocity vector based on a starting and ending
/// point, and a target velocity. The velocity vector is returned via the targetVelocityVector pointer.
/// Each pointer corresponds to a 3-dimensional array for XYZ.
void calcTargetVelocityVector(double *targetPos, double *origPos, double targetVelocity, double *targetVelocityVector)
{
    double lineVector[3], lineVectorMag;
    double unitVector[3];
    lineVector[0] = (targetPos[0] - origPos[0]);
    lineVector[1] = (targetPos[1] - origPos[1]);
    lineVector[2] = (targetPos[2] - origPos[2]);
    lineVectorMag = sqrt (pow (lineVector[0], 2) + pow (lineVector[1], 2) + pow (lineVector[2], 2));

    unitVector[0] = (lineVector[0] / lineVectorMag);
    unitVector[1] = (lineVector[1] / lineVectorMag);
    unitVector[2] = (lineVector[2] / lineVectorMag);

    targetVelocityVector[0] = (targetVelocity * unitVector[0]);
    targetVelocityVector[1] = (targetVelocity * unitVector[1]);
    targetVelocityVector[2] = (targetVelocity * unitVector[2]);
}

///This function will check if a motor is set for the correct target direction.
/// The "fwd" argument indicates if the direction should be forward, or reverse, given a
/// positive or negative input.
void checkSetMotorDir(struct motorController *motorBlock, double fwd)
{
    if (fwd > 0 && motorBlock->targetDir != motorBlock->fwdDir)
    {
        motorBlock->targetDir = motorBlock->fwdDir;
    }
    else if (fwd < 0)
    {
        if (motorBlock->targetDir == motorBlock->fwdDir && motorBlock->fwdDir == IOPORT_LEVEL_HIGH)
        {
            motorBlock->targetDir = IOPORT_LEVEL_LOW;
        }
        else if (motorBlock->targetDir == motorBlock->fwdDir && motorBlock->fwdDir == IOPORT_LEVEL_LOW)
        {
            motorBlock->targetDir = IOPORT_LEVEL_HIGH;
        }
    }
}

