/***************************************************************************
 *                     GVOHub.cpp - VO SAMP Hub class                      *
 * ----------------------------------------------------------------------- *
 *  copyright (C) 2014-2015 by Thierry Louge                               *
 * ----------------------------------------------------------------------- *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/
/**
 * @file GVOHub.cpp
 * @brief VO SAMP Hub class implementation
 * @author Thierry Louge
 */

/* __ Includes ___________________________________________________________ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <cstdlib>         // std::getenv() function
#include <cstdio>          // std::fopen(), etc. functions
#include <cstring>         // std::memset() function
#include <csignal>         // signal() function
#include <cerrno>          // errno
#include <unistd.h>        // close() function
#include <netdb.h>         // getaddrinfo() function
#include <netinet/in.h>    // sockaddr_in, INADDR_ANY, htons
#include <fstream>
#include <sys/shm.h>
#include <sys/socket.h>    // socket(), connect() functions
#include <sys/wait.h>      // waitpid() function
#include <arpa/inet.h>     // inet_addr() function
#include "GVOHub.hpp"
#include "GException.hpp"
#include "GTools.hpp"

/* __ Method name definitions ____________________________________________ */
#define G_START_HUB                                     "GVOHub::start_hub()"

/* __ Macros _____________________________________________________________ */

/* __ Coding definitions _________________________________________________ */

/* __ Debug definitions __________________________________________________ */
//#define G_CONSOLE_DUMP
#define G_SHOW_MESSAGE


/*==========================================================================
 =                                                                         =
 =                         Constructors/destructors                        =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Void constructor
 ***************************************************************************/
GVOHub::GVOHub(void)
{
    // Initialise members
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy constructor
 *
 * @param[in] hub VO Hub.
 ***************************************************************************/
GVOHub::GVOHub(const GVOHub& hub)
{
    // Initialise members
    init_members();

    // Copy members
    copy_members(hub);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Destructor
 ***************************************************************************/
GVOHub::~GVOHub(void)
{
    // Free members
    free_members();

    // Return
    return;
}


/*==========================================================================
 =                                                                         =
 =                                Operators                                =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Assignment operator
 *
 * @param[in] hub VO hub.
 * @return VO hub.
 ***************************************************************************/
GVOHub& GVOHub::operator=(const GVOHub& hub)
{
    // Execute only if object is not identical
    if (this != &hub) {

        // Free members
        free_members();

        // Initialise members
        init_members();

        // Copy members
        copy_members(hub);

    } // endif: object was not identical

    // Return
    return *this;
}


/*==========================================================================
 =                                                                         =
 =                             Public methods                              =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Clear object.
 *
 * Reset object to a clean initial state.
 ***************************************************************************/
void GVOHub::clear(void)
{
    // Free members
    free_members();

    // Initialise members
    init_members();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Clone object
 ***************************************************************************/
GVOHub* GVOHub::clone(void) const
{
    // Clone client
    return new GVOHub(*this);
}


/***********************************************************************//**
 * @brief Start Hub
 ***************************************************************************/
void GVOHub::start(void)
{
    // Create SAMP file
    create_samp_file();

    // Start Hub
    start_hub();

    // Return
    return;
}


/***********************************************************************//**
 * @brief Print VO hub information
 *
 * @param[in] chatter Chattiness (defaults to NORMAL).
 * @return String containing VO hub information
 ***************************************************************************/
std::string GVOHub::print(const GChatter& chatter) const
{
    // Initialise result string
    std::string result;

    // Continue only if chatter is not silent
    if (chatter != SILENT) {

        // Append header
        result.append("=== GVOHub ===");

        // Append Hub information
        result.append("\n"+gammalib::parformat("Hub identifier")+m_hub_id);
        result.append("\n"+gammalib::parformat("Hub key")+m_secret);
        result.append("\n"+gammalib::parformat("Hub URL")+m_hub_url);
        result.append("\n"+gammalib::parformat("Hub host")+m_hub_host);
        result.append("\n"+gammalib::parformat("Hub port")+m_hub_port);
        result.append("\n"+gammalib::parformat("Hub path")+m_hub_path);
        result.append("\n"+gammalib::parformat("SAMP protocol version"));
        result.append(m_version);

    } // endif: chatter was not silent

    // Return result
    return result;
}


/*==========================================================================
 =                                                                         =
 =                             Private methods                             =
 =                                                                         =
 ==========================================================================*/

/***********************************************************************//**
 * @brief Initialise class members
 ***************************************************************************/
void GVOHub::init_members(void)
{
    // Initialise members
    m_secret        = random_string(15);
    m_hub_host      = "127.0.0.1";
    m_hub_port      = "2526";
    m_hub_path      = "xmlrpc";
    m_hub_url       = "http://"+m_hub_host+":"+m_hub_port+"/"+m_hub_path;
    m_version       = "1.3";
    m_hub_id        = "gammalib_hub";
    m_socket        = -1;        // Signals no socket
    m_shutdown      = false;
    m_clients.clear();
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Copy class members
 *
 * @param[in] hub VO hub.
 ***************************************************************************/
void GVOHub::copy_members(const GVOHub& hub)
{
    // Copy members
    m_secret   = hub.m_secret;
    m_hub_url  = hub.m_hub_url;
    m_hub_host = hub.m_hub_host;
    m_hub_port = hub.m_hub_port;
    m_hub_path = hub.m_hub_path;
    m_version  = hub.m_version;
    m_hub_id   = hub.m_hub_id;
    m_socket   = hub.m_socket;
    m_shutdown = hub.m_shutdown;
    m_clients  = hub.m_clients;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Delete class members
 ***************************************************************************/
void GVOHub::free_members(void)
{ 
    // Close sockets
    if (m_socket != -1) {
        close(m_socket);
        m_socket = -1;
    }
    
    // Remove lockfile
    std::string lockurl = get_hub_lockfile();
    std::remove(lockurl.c_str());

    // Return
    return;
}


/***********************************************************************//**
 * @brief Starts the SAMP hub socket and listens on it
 *
 * @exception GException::runtime_error
 *            Problem with creating of or listening on socket
 *
 * This is the main Hub event loop.
 ***************************************************************************/
void GVOHub::start_hub(void)
{
    // Prepare TCP/IP structure
    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(m_hub_host.c_str());
    serv_addr.sin_port        = htons(gammalib::toint(m_hub_port));
    
    // Create Hub socket
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Creation of hub main socket
    if (m_socket < 0) {
        std::string msg = "Unable to create Hub socket. Errno="+
                          gammalib::str(errno);
        throw GException::runtime_error(G_START_HUB, msg);
    }
    
    // Set hub main socket to allow multiple connections
    int opt = 1;
    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
        std::string msg = "Unable to set Hub socket to multiple connections."
                          " Errno="+gammalib::str(errno);
        throw GException::runtime_error(G_START_HUB, msg);
    }
    #ifdef SO_REUSEPORT
    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEPORT, (char*)&opt, sizeof(opt)) < 0) {
        std::string msg = "Unable to set Hub socket to multiple connections."
                          " Errno="+gammalib::str(errno);
        throw GException::runtime_error(G_START_HUB, msg);
    }
    #endif
    
    // Server socket is opened. Now, bind it to the port, with family etc.
    if (bind(m_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::string msg = "Unable to bind Hub socket to server socket. Errno="+
                          gammalib::str(errno);
        throw GException::runtime_error(G_START_HUB, msg);
    }

    // Now start listening for the clients: 5 requests simultaneously pending
    // at maximum
    if (listen(m_socket, 5) < 0) {
        std::string msg = "Unable to start listening on Hub socket. Errno="+
                          gammalib::str(errno);
        throw GException::runtime_error(G_START_HUB, msg);
    }

    // ...
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    // Main event handling loop
    while (1) {

        // Accept connection from the client 
    	socklen_t socket = accept(m_socket, (struct sockaddr *)&cli_addr, &clilen);
    	if (socket < 0) {
            std::string msg = "Client connection to socket not accepted.";
            throw GException::runtime_error(G_START_HUB, msg);
    	}

        // Handle request
        handle_request(socket);

        // Close socket
        close(socket);

        // If we have received a shutdown request then exit the event
        // loop
        if (m_shutdown) {
            break;
        }

    } // endwhile: main event loop

    // Close socket
    close(m_socket);
    m_socket = -1;

    // Return
    return;
}


/***********************************************************************//**
 * @brief Reads the client message and runs appropriate function
 *
 * @param[in] sock Socket to client.
 ***************************************************************************/
void GVOHub::handle_request(const socklen_t& sock)
{
    // Header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::handle_request" << std::endl;
    #endif

    // Initialize buffer
    char buffer[1001];

    // Initialise message
    std::string message = "";

    // Read from socket until nothing is received anymore.
    int timeout = 2000; // Initial timeout is 2 sec
    int n       = 0;
    do {
        n = gammalib::recv(sock, buffer, 1000, 0, timeout);
        if (n > 0) {
            buffer[n] = '\0';
            message.append(std::string(buffer));
        }
        timeout = 10; // The timeout now is 0.01 sec 
    } while (n > 0);
    
    // Dump the buffer
    #if defined(G_SHOW_MESSAGE)
    std::cout << std::endl;
    std::cout << "GVOHub has received the following message:" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << message << std::endl;
    #endif
    
    // Extract response into an XML object
    GXml   xml;
    size_t start = message.find("<?xml");
    if (start != std::string::npos) {
        xml = GXml(message.substr(start, std::string::npos));
    }

    // Get methodName value
    std::string method_called;
    const GXmlNode* node = xml.element("methodCall > methodName");
    if (node != NULL) {
        const GXmlText* text = static_cast<const GXmlText*>((*node)[0]);
        if (text != NULL) {
            method_called = text->text();
        }
    }

    // Dump the buffer
    #if defined(G_CONSOLE_DUMP)
    std::cout << "Method called: " << method_called << std::endl;
    #endif

    // Dispatch according to method
    if (method_called.compare("samp.hub.ping") == 0) {
        request_ping(sock);
    }
    else if (method_called.compare("samp.hub.register") == 0) {
        request_register(xml, sock);
    }
    else if (method_called.compare("samp.hub.unregister") == 0) {
        request_unregister(xml, sock);
    }
    else if (method_called.compare("samp.hub.declareMetadata") == 0) {
        request_declare_metadata(xml, sock);
    }
    else if (method_called.compare("samp.hub.declareSubscriptions") == 0) {
        request_declare_subscriptions(xml, sock);
    }
    else if (method_called.compare("samp.hub.setXmlrpcCallback") == 0) {
        request_set_xml_rpc_callback(xml, sock);
    }
    else if (method_called.compare("samp.hub.getSubscriptions") == 0) {
        request_get_subscriptions(xml, sock);
    }
    else if (method_called.compare("samp.hub.getRegisteredClients") == 0) {
        request_get_registered_clients(xml, sock);
    }
    else if (method_called.compare("samp.hub.getSubscribedClients") == 0) {
        request_get_subscribed_clients(xml, sock);
    }
    else if (method_called.compare("samp.hub.getMetadata") == 0) {
        request_get_metadata(xml, sock);
    }
    else if (method_called.compare("samp.hub.notify") == 0) {
        request_ping(sock);
    }
    else if (method_called.compare("samp.hub.notifyAll") == 0) {
        request_ping(sock);
    }
    else if (method_called.compare("samp.hub.call") == 0) {
        request_ping(sock);
    }
    else if (method_called.compare("samp.hub.callAll") == 0) {
        request_ping(sock);
    }
    else if (method_called.compare("samp.hub.callAndWait") == 0) {
        request_ping(sock);
    }
    else if (method_called.compare("samp.hub.reply") == 0) {
        request_ping(sock);
    }
    else if (method_called.compare("samp.hub.shutdown") == 0) {
        request_shutdown(sock);
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Handles ping requests
 *
 * @param[in] sock Socket.
 *
 * Handles all incoming ping requests.
 ***************************************************************************/
void GVOHub::request_ping(const socklen_t& sock)
{
    // Header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::request_ping" << std::endl;
    #endif
    
    // Post void message
    post_samp_void(sock);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Handles registration requests
 *
 * @param[in] xml XML message sent by client.
 * @param[in] sock Socket.
 *
 * Handles all incoming registration requests.
 ***************************************************************************/
void GVOHub::request_register(const GXml& xml, const socklen_t& sock)
{
    // Header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::request_register" << std::endl;
    #endif

    // Set client reference
    int         counter = 0;
    std::string reference;
    do {
        reference = "c"+gammalib::str(counter);
        int i = 0;
        for (; i < m_clients.size(); ++i) {
            if (m_clients[i].reference == reference) {
                counter++;
                break;
            }
        }
        if (i >= m_clients.size()) {
            break;
        }
    } while (true);

    // Create a new client
    struct client voclient;
    voclient.reference   = reference;
    voclient.private_key = random_string(15);
    
    // Attached client
    m_clients.push_back(voclient);

    // Declare response
    std::string response = "";

    // Set response
    response.append("<?xml version='1.0' encoding='UTF-8'?>\n");
    response.append("<methodResponse>\n");
    response.append("<params>\n");
    response.append("  <param><value><struct>\n");
    response.append("    <member>\n");
    response.append("      <name>samp.private-key</name>\n");
    response.append("      <value>"+voclient.private_key+"</value>\n");
    response.append("    </member>\n");
    response.append("    <member>\n");
    response.append("      <name>samp.hub-id</name>\n");
    response.append("      <value>"+m_hub_id+"</value>\n");
    response.append("    </member>\n");
    response.append("    <member>\n");
    response.append("      <name>samp.self-id</name>\n");
    response.append("      <value>"+voclient.reference+"</value>\n");
    response.append("    </member>\n");
    response.append("    <member>\n");
    response.append("      <name>samp.status</name>\n");
    response.append("      <value>samp.ok</value>\n");
    response.append("    </member>\n");
    response.append("    <member>\n");
    response.append("      <name>samp.url-translator</name>\n");
    response.append("      <value>"+m_hub_url+"</value>\n");
    response.append("    </member>\n");
    response.append("  </struct></value></param>\n");
    response.append("</params>\n");
    response.append("</methodResponse>\n");

    // Post response
    post_string(response, sock);

    // Send notifications
    send_notifications("samp.hub.event.register", voclient.reference);	

    //Return
    return;
}


/***********************************************************************//**
 * @brief Handles unregistration requests
 *
 * @param[in] xml XML message sent by client.
 * @param[in] sock Socket.
 *
 * Handles all incoming unregistration requests.
 ***************************************************************************/
void GVOHub::request_unregister(const GXml& xml, const socklen_t& sock)
{
    // Optionally dump header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::request_unregister" << std::endl;
    #endif 

    // Get client index
    int i = get_client_index(xml);

    // Continue only if index is valid
    if (i != -1) {

        // Post void message
        post_samp_void(sock);

        // Send notifications
        send_notifications("samp.hub.event.unregister",
                           m_clients[i].reference);

        // Remove client
        m_clients.erase(m_clients.begin()+i);
        
    } // endif: valid client found

    // Signal if the client was unknown
    #if defined(G_CONSOLE_DUMP)
    else {
        std::cout << " *** ERROR: Unable to find client " << get_client_key(xml) << std::endl;
    }
    #endif
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Handles metadata declaration requests
 *
 * @param[in] xml XML message sent by client.
 * @param[in] sock Socket.
 *
 * Handles all incoming metadata declaration requests.
 ***************************************************************************/
void GVOHub::request_declare_metadata(const GXml& xml, const socklen_t& sock)
{
    // Header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::request_declare_metadata" << std::endl;
    #endif

    // Get client index
    int i = get_client_index(xml);

    // Continue only if index is valid
    if (i != -1) {

        // Store metadata
        m_clients[i].name          = get_response_value(xml, "samp.name");;
        m_clients[i].description   = get_response_value(xml, "samp.description.text");
        m_clients[i].icon          = get_response_value(xml, "samp.icon.url");
        m_clients[i].documentation = get_response_value(xml, "samp.documentation.url");
        m_clients[i].affiliation   = get_response_value(xml, "author.affiliation");
        m_clients[i].email         = get_response_value(xml, "author.email");
        m_clients[i].author_name   = get_response_value(xml, "author.name");
        m_clients[i].homepage      = get_response_value(xml, "home.page");

        // Post void message
        post_samp_void(sock);
            
        // Send notifications
        send_notifications("samp.hub.event.metadata", m_clients[i].reference);

    } // endif: client found
        
    // Signal if the client was unknown
    #if defined(G_CONSOLE_DUMP)
    else {
        std::cout << " *** ERROR: Unable to find client " << get_client_key(xml) << std::endl;
    }
    #endif
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Handles subscriptions declaration requests
 *
 * @param[in] xml XML message sent by client.
 * @param[in] sock Socket.
 *
 * Handles all incoming subscription declaration requests.
 ***************************************************************************/
void GVOHub::request_declare_subscriptions(const GXml& xml,const socklen_t& sock)
{
    // Header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::request_declare_subscriptions" << std::endl;
    #endif

    // Get client index
    int i = get_client_index(xml);

    // Continue only if index is valid
    if (i != -1) {

        // Get subscriptions
        std::vector<std::string> subscriptions = get_subscriptions(xml);

        // Store subscriptions
        for (int k = 0; k < subscriptions.size(); ++k) {
            m_clients[i].subscriptions.push_back(subscriptions[k]);
        }

        // Post SAMP ok
        post_samp_ok(sock);
        
    } // endif: valid client found

    // Signal if the client was unknown
    #if defined(G_CONSOLE_DUMP)
    else {
        std::cout << " *** ERROR: Unable to find client " << get_client_key(xml) << std::endl;
    }
    #endif
    
    //Return
    return;
}


/***********************************************************************//**
 * @brief Handles XML-RPC callback setting requests
 *
 * @param[in] xml XML message sent by client.
 * @param[in] sock Socket.
 *
 * Handles all incoming XML-RPC callback setting requests.
 ***************************************************************************/
void GVOHub::request_set_xml_rpc_callback(const GXml&      xml,
                                          const socklen_t& sock)
{
    // Header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::request_set_xml_rpc_callback" << std::endl;
    #endif

    // Get client index
    int i = get_client_index(xml);

    // Continue only if index is valid
    if (i != -1) {

        // Get callback URL
        std::string client_url = get_callback_url(xml);

        // Set callback URL
        m_clients[i].url = get_callback_url(xml);

        // Post SAMP ok
        post_samp_ok(sock);

    } // endif: client found

    // Signal if the client was unknown
    #if defined(G_CONSOLE_DUMP)
    else {
        std::cout << " *** ERROR: Unable to find client " << get_client_key(xml) << std::endl;
    }
    #endif

    // Return
    return;
}


/***********************************************************************//**
 * @brief Handles subscriptions getting requests
 *
 * @param[in] xml XML message sent by client.
 * @param[in] sock Socket.
 *
 * Handles all incoming subscriptions getting requests.
 ***************************************************************************/
void GVOHub::request_get_subscriptions(const GXml& xml, const socklen_t& sock)
{
    // Header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::request_get_subscriptions" << std::endl;
    #endif

    // Initialise client name
    std::string client_name = "";

    // Get the client name
    const GXmlNode* node = xml.element("methodCall > params > param[1] > value");
    if (node != NULL) {
        const GXmlText* text = static_cast<const GXmlText*>((*node)[0]);
        if (text != NULL) {
            client_name = text->text();
        }
    }

    // Declare response
    std::string response = "";

    // Set response
    response.append("<?xml version='1.0' encoding='UTF-8'?>\n");
    response.append("<methodResponse>\n");
    response.append("<params>\n");
    response.append("  <param><value><struct>\n");

    // If key is "gammalib_hub" then return some special information
    if (client_name == "gammalib_hub") {
        // Do nothing
    }
    
    // ... handle all other clients
    else {

        // Get client index
        int i = get_client_index(client_name);

        // Continue only if index is valid
        if (i != -1) {

            // Append all subscriptions
            for (int k = 0; k < m_clients[i].subscriptions.size(); ++k) {
                response.append("    <member>\n");
                response.append("      <name>");
                response.append(m_clients[i].subscriptions[k]);
                response.append("</name>\n");
                response.append("      <value></value>\n");
                response.append("    </member>\n");
            }
            
        }

        // Signal if the client was unknown
        #if defined(G_CONSOLE_DUMP)
        else {
            std::cout << " *** ERROR: Unable to find client " << get_client_key(xml) << std::endl;
        }
        #endif
        
    }

    // Finish response
    response.append("  </struct></value></param>\n");
    response.append("</params>\n");
    response.append("</methodResponse>\n");
        
    // Post response
    post_string(response, sock);
   
    // Return
    return;
}


/***********************************************************************//**
 * @brief Handles registered client information requests
 *
 * @param[in] xml XML message sent by client.
 * @param[in] sock Socket.
 *
 * Handles all incoming registered client information requests.
 ***************************************************************************/
void GVOHub::request_get_registered_clients(const GXml& xml, const socklen_t& sock)
{
    // Header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::request_get_registered_clients" << std::endl;
    #endif

    // Get client key
    std::string key = get_client_key(xml);

    // Declare message
    std::string msg = "";

    // Set response
    msg.append("<?xml version=\'1.0\' encoding=\"UTF-8\"?>\n");
    msg.append("<methodResponse>\n");
    msg.append("<params>\n");
    msg.append("  <param><value><array><data>\n");
    msg.append("    <value>gammalib_hub</value>\n");

    // Loop over all clients. Do not send back current client's registration
    for (int i = 0; i < m_clients.size(); ++i) {
        if (m_clients[i].private_key != key) {
            msg.append("    <value>");
            msg.append(m_clients[i].reference);
            msg.append("</value>\n");
        }
    }
    
    // Finish response
    msg.append("  </data></array></value></param>\n");
    msg.append("</params>\n");
    msg.append("</methodResponse>\n");

    // Post response
    post_string(msg, sock);
    
    // Return
    return;
  
}


/***********************************************************************//**
 * @brief Handles subscribed client information requests
 *
 * @param[in] xml XML message sent by client.
 * @param[in] sock Socket.
 *
 * Handles all incoming subscribed client information requests.
 ***************************************************************************/
void GVOHub::request_get_subscribed_clients(const GXml& xml, const socklen_t& sock)
{
    // Header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::request_get_subscribed_clients" << std::endl;
    #endif

/*
    std::string buffer;
    const int n = 1000; 
    char      line[n];
    FILE* fptr = fopen("SAMP-registered.txt", "r");
    if (fptr != NULL) {
        while (fgets(line, n, fptr) != NULL) {

                // Convert line to C++ string
                std::string cline = std::string(line);

                // Check for secret key
                if (cline.compare(0, 10, "Registered") == 0) {
                    buffer = gammalib::strip_chars(cline.substr(10, std::string::npos), "\r\n");
                }
    }
    
    // Closes file
    fclose(fptr);
*/

    // Post void message
    post_samp_void(sock);
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Handles a metadata requests
 *
 * @param[in] xml XML message sent by client.
 * @param[in] sock Socket.
 *
 * Handles all metadata requests.
 ***************************************************************************/
void GVOHub::request_get_metadata(const GXml& xml, const socklen_t& sock)
{
    // Header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::request_get_metadata" << std::endl;
    #endif

    // Initialise client name
    std::string client_name = "";

    // Get the client name
    const GXmlNode* node = xml.element("methodCall > params > param[1] > value");
    if (node != NULL) {
        const GXmlText* text = static_cast<const GXmlText*>((*node)[0]);
        if (text != NULL) {
            client_name = text->text();
        }
    }

    // Declare response
    std::string response = "";

    // Set response
    response.append("<?xml version='1.0' encoding='UTF-8'?>\n");
    response.append("<methodResponse>\n");
    response.append("<params>\n");
    response.append("  <param><value><struct>\n");
    response.append("    <member>\n");
    response.append("      <name>samp.status</name>\n");
    response.append("      <value>samp.ok</value>\n");
    response.append("    </member>\n");

    // If key is "gammalib_hub" then return some special information
    if (client_name == "gammalib_hub") {
		response.append("    <member>\n");
        response.append("      <name>samp.name</name>\n");
        response.append("      <value>gammalib_hub</value>\n");
        response.append("    </member>\n");
   		response.append("    <member>\n");
        response.append("      <name>samp.description.text</name>\n");
        response.append("      <value>GammaLib VO Hub</value>\n");
        response.append("    </member>\n");
		response.append("    <member>\n");
        response.append("      <name>samp.icon.url</name>\n");
        response.append("      <value>http://a.fsdn.com/allura/p/gammalib/icon</value>\n");
        response.append("    </member>\n");
		response.append("    <member>\n");
        response.append("      <name>samp.documentation.url</name>\n");
        response.append("      <value>http://cta.irap.omp.eu/gammalib/user_manual/modules/vo.html</value>\n");
        response.append("    </member>\n");
		response.append("    <member>\n");
        response.append("      <name>author.affiliation</name>\n");
        response.append("      <value>IRAP, Toulouse, France</value>\n");
        response.append("    </member>\n");
		response.append("    <member>\n");
        response.append("      <name>author.email</name>\n");
        response.append("      <value>jurgen.knodlseder@irap.omp.eu</value>\n");
        response.append("    </member>\n");
		response.append("    <member>\n");
        response.append("      <name>author.name</name>\n");
        response.append("      <value>J. Knoedlseder, T. Louge</value>\n");
        response.append("    </member>\n");
		response.append("    <member>\n");
        response.append("      <name>home.page</name>\n");
        response.append("      <value>http://cta.irap.omp.eu/gammalib/</value>\n");
        response.append("    </member>\n");
    }

    // ... otherwise get the index
    else {
 
        // Get client index
        int i = get_client_index(client_name);

        // Continue only if index is valid
        if (i != -1) {

            // Append response
            response.append("    <member>\n");
            response.append("      <name>samp.name</name>\n");
            response.append("      <value>"+m_clients[i].name+"</value>\n");
            response.append("    </member>\n");
            response.append("    <member>\n");
            response.append("      <name>samp.description.text</name>\n");
            response.append("      <value>"+m_clients[i].description+"</value>\n");
            response.append("    </member>\n");
            response.append("    <member>\n");
            response.append("      <name>samp.icon.url</name>\n");
            response.append("      <value>"+m_clients[i].icon+"</value>\n");
            response.append("    </member>\n");
            response.append("    <member>\n");
            response.append("      <name>samp.documentation.url</name>\n");
            response.append("      <value>"+m_clients[i].documentation+"</value>\n");
            response.append("    </member>\n");
            response.append("    <member>\n");
            response.append("      <name>author.affiliation</name>\n");
            response.append("      <value>"+m_clients[i].affiliation+"</value>\n");
            response.append("    </member>\n");
            response.append("    <member>\n");
            response.append("      <name>author.email</name>\n");
            response.append("      <value>"+m_clients[i].email+"</value>\n");
            response.append("    </member>\n");
            response.append("    <member>\n");
            response.append("      <name>author.name</name>\n");
            response.append("      <value>"+m_clients[i].author_name+"</value>\n");
            response.append("    </member>\n");
            response.append("    <member>\n");
            response.append("      <name>home.page</name>\n");
            response.append("      <value>"+m_clients[i].homepage+"</value>\n");
            response.append("    </member>\n");

        } // endif: index was valid
        
    } // endelse: client was not the hub

    // Finish response
    response.append("  </struct></value></param>\n");
    response.append("</params>\n");
    response.append("</methodResponse>\n");
    
    // Post response
    post_string(response, sock);
   
    // Return
    return;
}


/***********************************************************************//**
 * @brief Handles Hub shutdown requests
 *
 * @param[in] sock Socket.
 *
 * Handles all incoming Hub shutdown requests.
 ***************************************************************************/
void GVOHub::request_shutdown(const socklen_t& sock)
{
    // Header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::request_shutdown" << std::endl;
    #endif

    // Set shutdown flag
    m_shutdown = true;
    
    // Post SAMP ok
    post_samp_ok(sock);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Extract client key from XML request
 *
 * @param[in] xml XML message sent by client.
 *
 * Extracts the clients key from the XML request.
 ***************************************************************************/
std::string GVOHub::get_client_key(const GXml& xml) const
{
    // Header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::get_client_key" << std::endl;
    #endif

    // Initialise response
    std::string client_key = "";

    // Get the client's private key
    const GXmlNode* node = xml.element("methodCall > params > param > value");
    if (node != NULL) {
        const GXmlText* text = static_cast<const GXmlText*>((*node)[0]);
        if (text != NULL) {
            client_key = text->text();
        }
    }

    // Return key
    return client_key;
}


/***********************************************************************//**
 * @brief Extract client index in shared memory from XML request
 *
 * @param[in] xml XML message sent by client.
 * @return Client index (-1 if not found).
 *
 * Extracts the client index from the XML request. The method returns -1 if
 * no client was found.
 ***************************************************************************/
int GVOHub::get_client_index(const GXml& xml) const
{
    // Header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::get_client_index" << std::endl;
    #endif

    // Initialise index
    int index = -1;

    // Get the client's private key
    std::string key = get_client_key(xml);

    // Continue only if string is not empty
    if (!key.empty()) {

        // Searches for key in clients
        for (int i = 0; i < m_clients.size(); ++i) {
            if (m_clients[i].private_key == key) {
                index = i;
                break;
            }
        }

    } // endif: key was not empty

    // Return index
    return index;
}


/***********************************************************************//**
 * @brief Extract client index in shared memory from XML request
 *
 * @param[in] reference Client reference.
 * @return Client index (-1 if not found).
 *
 * Extracts the client index from the client reference. The method returns -1
 * if no client was found.
 ***************************************************************************/
int GVOHub::get_client_index(const std::string& reference) const
{
    // Header
    #if defined(G_CONSOLE_DUMP)
    std::cout << "GVOHub::get_client_index" << std::endl;
    #endif

    // Initialise index
    int index = -1;

    // Search for index
    for (int i = 0; i < m_clients.size(); ++i) {
        if (m_clients[i].reference == reference) {
            index = i;
            break;
        }
    }

    // Return index
    return index;
}


/***********************************************************************//**
 * @brief Returns value for a SAMP client query parameter
 *
 * @param[in] xml client query XML document.
 * @param[in] name Parameter name.
 * @return Parameter value.
 *
 * Returns value for a SAMP client query parameter. If the specified
 * parameter was not found or if the response structure is not compliant,
 * an empty string is returned.
 ***************************************************************************/
std::string GVOHub::get_response_value(const GXml&        xml,
                                       const std::string& name) const
{
    // Declare value
    std::string value = "";

    // Get the client's private key
    const GXmlNode* node = xml.element("methodCall > params > param[1] > value > struct");
    if (node != NULL) {
        int num = node->elements("member");            
        for (int i = 0; i < num; ++i) {
            const GXmlNode* member = node->element("member", i);
            std::string one_name;
            std::string one_value;
            get_name_value_pair(member, one_name, one_value);
            if (one_name == name) {
                value = one_value;
                break;
            }
        }
        
    }

    // Return value
    return value;
}


/***********************************************************************//**
 * @brief Extract name / value pair from XML node
 *
 * @param[in] node Pointer to XML node.
 * @param[out] name Name string.
 * @param[out] value Value string.
 *
 * Extracts a name / value pair from a XML node. If the XML node pointer is
 * NULL, the name and value strings will be empty.
 ***************************************************************************/
void GVOHub::get_name_value_pair(const GXmlNode* node,
                                 std::string&    name,
                                 std::string&    value) const
{
    // Clear name and value strings
    name.clear();
    value.clear();

    // Continue only if node is valid
    if (node != NULL) {

        // Get name node and extract text content
        const GXmlNode* ptr = node->element("name", 0);
        if (ptr != NULL) {
            const GXmlText* text = static_cast<const GXmlText*>((*ptr)[0]);
            if (text != NULL) {
                name = text->text();
            }
        }

        // Get value node and extract text content
        ptr = node->element("value", 0);
        if (ptr != NULL) {
            const GXmlText* text = static_cast<const GXmlText*>((*ptr)[0]);
            if (text != NULL) {
                value = text->text();
            }
        }
    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Returns subscriptions from XML document
 *
 * @param[in] xml client query XML document.
 * @return List of subscriptions.
 ***************************************************************************/
std::vector<std::string> GVOHub::get_subscriptions(const GXml& xml) const                                         
{
    // Declare subscriptions
    std::vector<std::string> subscriptions;

    // Get the client's private key
    const GXmlNode* node = xml.element("methodCall > params > param[1] > value > struct");
    if (node != NULL) {
        int num = node->elements("member");
        for (int i = 0; i < num; ++i) {
            const GXmlNode* member = node->element("member", i);
            if (member != NULL) {
                const GXmlNode* name = member->element("name", 0);
                if (name != NULL) {
                    const GXmlText* text = static_cast<const GXmlText*>((*name)[0]);
                    if (text != NULL) {
                        subscriptions.push_back(text->text());
                    }
                }
            }
        }
    }

    // Return subscriptions
    return subscriptions;
}


/***********************************************************************//**
 * @brief Returns callback URL of client
 *
 * @param[in] xml client query XML document.
 * @return Callback URL of client.
 ***************************************************************************/
std::string GVOHub::get_callback_url(const GXml& xml) const                                         
{
    // Declare URL
    std::string url;

    // Get the client's private key
    const GXmlNode* node = xml.element("methodCall > params > param[1] > value");
    if (node != NULL) {
		const GXmlText* text = static_cast<const GXmlText*>((*node)[0]);
        if (text != NULL) {
		    url.append(text->text());
        }
    }

    // Return URL
    return url;
}


/***********************************************************************//**
 * @brief Returns SAMP Hub lockfile URL
 *
 * @return SAMP Hub lockfile URL (empty if no lockfile was found).
 *
 * Implements IVOA standard REC-SAMP-1.3-20120411.
 ***************************************************************************/
std::string GVOHub::get_hub_lockfile(void) const
{
    // Initialise result
    std::string url = "";

    // Check for existence of the SAMP_HUB environment variable first
    char* hub_ptr = std::getenv("SAMP_HUB");
    if (hub_ptr != NULL) {

        // Check for mandatory std-lockurl: prefix (no other prefixe is
        // supported so far)
        std::string lockurl = std::string(hub_ptr);
        if (lockurl.compare(0, 12, "std-lockurl:") == 0) {

            // Extract URL
            url = lockurl.substr(12, std::string::npos);

        } // endif: std-lockurl: prefix found
  
    }

    // ... otherwise the lockfile should be $HOME/.samp
    else {

        // Get user's HOME directory path as the prefix of the full
        // path. If the HOME environment variable is not set we
        // expect that .samp is in the local directory. This is non
        // standard, but prevents for creating an exception here.
        std::string prefix = "";
        char* home_ptr = std::getenv("HOME");
        if (home_ptr != NULL) {
            prefix = std::string(home_ptr) + "/";
        }

        // Set filename
        url = prefix + ".samp";

    } // endelse: no SAMP_HUB environment variable found

    // Return URL
    return url;
}


/***********************************************************************//**
 * @brief Send notifications
 *
 * @param[in] method Name of the method
 * @param[in] client Reference of the client
 *
 * The method send notifications to all registered clients that request so.
 ***************************************************************************/
void GVOHub::send_notifications(const std::string& method,
                                const std::string& client)
{
    // Loop over all clients
    for (int i = 0; i < m_clients.size(); ++i) {
    
        // Loop over all methods
        for (int j = 0; j < m_clients[i].subscriptions.size(); ++j) {

            // Continue only if method matches
            if (method == m_clients[i].subscriptions[j]) {
    
                // Declare notification
                std::string msg = "";

                // Register
                if (method == "samp.hub.event.register") {

                    // Set response for samp.hub.event.register
                    msg.append("<?xml version=\"1.0\"?>\n");
                    msg.append("<methodCall>\n");
                    msg.append("<methodName>samp.client.receiveNotification</methodName>\n");
                    msg.append("<params>\n");
                    msg.append("  <param><value>");
                    msg.append(m_clients[i].private_key);
                    msg.append("</value></param>\n");
                    msg.append("  <param><value>"+m_hub_id+"</value></param>\n");
                    msg.append("  <param><value><struct>\n");
                    msg.append("    <member>\n");
                    msg.append("      <name>samp.mtype</name>\n");
                    msg.append("      <value>samp.hub.event.register</value>\n");
                    msg.append("    </member>\n");
                    msg.append("    <member>\n");
                    msg.append("      <name>samp.params</name>\n");
                    msg.append("      <value><struct>\n");
                    msg.append("        <member>\n");
                    msg.append("          <name>id</name>\n");
                    msg.append("          <value>"+client+"</value>\n");
                    msg.append("        </member>\n");
                    msg.append("      </struct></value>\n");
                    msg.append("    </member>\n");
                    msg.append("  </struct></value></param>\n");
                    msg.append("</params>\n");
                    msg.append("</methodCall>\n");
                    
                    // Post notification
                    notify(m_clients[i].url, msg);
                    
                }
                    
                // Unregister
                else if (method == "samp.hub.event.unregister") {

                    // Set response for samp.hub.event.unregister
                    msg.append("<?xml version=\"1.0\"?>\n");
                    msg.append("<methodCall>\n");
                    msg.append("<methodName>samp.client.receiveNotification</methodName>\n");
                    msg.append("<params>\n");
                    msg.append("  <param><value>");
                    msg.append(m_clients[i].private_key);
                    msg.append("</value></param>\n");
                    msg.append("  <param><value>");
                    msg.append(m_hub_id);
                    msg.append("</value></param>\n");
                    msg.append("  <param><value><struct>\n");
                    msg.append("    <member>\n");
                    msg.append("      <name>samp.mtype</name>\n");
                    msg.append("      <value>samp.hub.event.unregister</value>\n");
                    msg.append("    </member>\n");
                    msg.append("    <member>\n");
                    msg.append("      <name>samp.params</name>\n");
                    msg.append("      <value><struct>\n");
                    msg.append("        <member>\n");
                    msg.append("          <name>id</name>\n");
                    msg.append("          <value>"+client+"</value>\n");
                    msg.append("        </member>\n");
                    msg.append("      </struct></value>\n");
                    msg.append("    </member>\n");
                    msg.append("  </struct></value></param>\n");
                    msg.append("</params>\n");
                    msg.append("</methodCall>\n");
                        
                    // Post notification
                    notify(m_clients[i].url, msg);
					
                }
                    
                // Metadata
			    else if (method == "samp.hub.event.metadata") {

                    // Get index of client
                    int k;
                    for (k = 0; k < m_clients.size(); ++k) {
                        if (m_clients[k].reference == client) {
                            break;
                        }
                    }

                    // Fall through if client index is not valid
                    if (k >= m_clients.size()) {
                        continue;
                    }

                    // Set response for samp.hub.event.metadata
                    msg.append("<?xml version=\"1.0\"?>\n");
                    msg.append("<methodCall><methodName>samp.client.receiveNotification</methodName>\n");
                    msg.append("<params>\n");
                    msg.append("  <param><value>");
                    msg.append(m_clients[i].private_key);
                    msg.append("</value></param>\n");
                    msg.append("  <param><value>"+m_hub_id+"</value></param>\n");
                    msg.append("  <param><value><struct>\n");
                    msg.append("    <member>\n");
                    msg.append("      <name>samp.mtype</name>\n");
                    msg.append("      <value>samp.hub.event.metadata</value>\n");
                    msg.append("    </member>\n");
                    msg.append("    <member>\n");
                    msg.append("      <name>samp.params</name>\n");
                    msg.append("      <value><struct>\n");
                    msg.append("        <member>\n");
                    msg.append("          <name>id</name>\n");
                    msg.append("          <value>"+m_clients[k].reference+"</value>\n");
                    msg.append("        </member>\n");
                    msg.append("        <member>\n");
                    msg.append("          <name>metadata</name>\n");
                    msg.append("          <value><struct>\n");
                    msg.append("            <member>\n");
                    msg.append("              <name>samp.name</name>\n");
                    msg.append("              <value>"+m_clients[k].name+"</value>\n");
                    msg.append("            </member>\n");
                    msg.append("            <member>\n");
                    msg.append("              <name>samp.description.text</name>\n");
                    msg.append("              <value>"+m_clients[k].description+"</value>\n");
                    msg.append("            </member>\n");
                    msg.append("            <member>\n");
                    msg.append("              <name>samp.icon.url</name>\n");
                    msg.append("              <value>"+m_clients[k].icon+"</value>\n");
                    msg.append("            </member>\n");
                    msg.append("            <member>\n");
                    msg.append("              <name>samp.documentation.url</name>\n");
                    msg.append("              <value>"+m_clients[k].documentation+"</value>\n");
                    msg.append("            </member>\n");
                    msg.append("            <member>\n");
                    msg.append("              <name>author.affiliation</name>\n");
                    msg.append("              <value>"+m_clients[k].affiliation+"</value>\n");
                    msg.append("            </member>\n");
                    msg.append("            <member>\n");
                    msg.append("              <name>author.name</name>\n");
                    msg.append("              <value>"+m_clients[k].author_name+"</value>\n");
                    msg.append("            </member>\n");
                    msg.append("            <member>\n");
                    msg.append("              <name>author.email</name>\n");
                    msg.append("              <value>"+m_clients[k].email+"</value>\n");
                    msg.append("            </member>\n");
                    msg.append("            <member>\n");
                    msg.append("              <name>home.page</name>\n");
                    msg.append("              <value>"+m_clients[k].homepage+"</value>\n");
                    msg.append("            </member>\n");
                    msg.append("          </struct></value>\n");
                    msg.append("        </member>\n");				
                    msg.append("      </struct></value>\n");
                    msg.append("    </member>\n");
                    msg.append("  </struct></value></param>\n");
                    msg.append("</params>\n");
                    msg.append("</methodCall>\n");
                        
                    // Post notification
                    notify(m_clients[i].url, msg);
					
                } // endelse: metadata

            } // endif: method was requested one

        } // endfor: looped over all methods

    } // endfor: looped over all clients
    
    // Return
    return;
}


/***********************************************************************//**
 * @brief Creates the lockfile, fill it
 *
 * Implements IVOA standard REC-SAMP-1.3-20120411.
 ***************************************************************************/
void GVOHub::create_samp_file(void) const
{
    // Get lockfile URL
    std::string lockurl = get_hub_lockfile();

    // Open SAMP lockfile. Continue only if opening was successful
    FILE* fptr = fopen(lockurl.c_str(), "w");

    // Successful?
    if (fptr != NULL) {

        // Write lockfile
        fprintf(fptr, "# SAMP lockfile\n");
        fprintf(fptr, "# Required keys:\n");
        fprintf(fptr, "samp.secret=%s\n", m_secret.c_str());
        fprintf(fptr, "samp.hub.xmlrpc.url=%s\n", m_hub_url.c_str());
        fprintf(fptr, "samp.profile.version=%s\n", m_version.c_str());
        fprintf(fptr, "# Info stored by hub for some private reason:\n");
        fprintf(fptr, "gammalib.hubid=%s\n", m_hub_id.c_str());
        
        // Close lockfile
        fclose(fptr);

    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Post string content to client
 *
 * @param[in] content String content to post.
 * @param[in] sock Socket.
 *
 * Posts the content of a string to a client.
 ***************************************************************************/
void GVOHub::post_string(const std::string& content, const socklen_t& sock) const
{
    // Continue only if socket is valid
    if (sock != -1) {

        // Determine content length
        int length = content.length();

        // Set prefix (note that the response has no POST!)
        std::string prefix = "HTTP/1.1 200 OK\n"
                             "Connection: close\n"
                             "Content-Type: text/xml\n"
                             "Content-Length: "+gammalib::str(length)+"\n\n";


        // Build post string
        std::string post = prefix + content;

        // Dump message to post
        #if defined(G_SHOW_MESSAGE)
        std::cout << std::endl;
        std::cout << "GVOHub response:" << std::endl;
        std::cout << "================" << std::endl;
        std::cout << post << std::endl;
        #endif

        // Send content to socket
        bool done = false;
        do {
            int length      = post.length();
            int sent_length = send(sock, post.c_str(), length, 0);
            if (sent_length < length) {
                post = post.substr(sent_length, std::string::npos);
            }
            else {
                done = true;
            }
        } while (!done);
	
    } // endif: socket was valid

    // Return
    return;
}


/***********************************************************************//**
 * @brief Post SAMP ok massage to client
 *
 * @param[in] sock Socket.
 *
 * Posts a SAMP OK message to a client.
 ***************************************************************************/
void GVOHub::post_samp_ok(const socklen_t& sock) const
{
    // Declare response
    std::string response = "";

    // Compose response
    response.append("<?xml version='1.0' encoding='UTF-8'?>\n");
    response.append("<methodResponse>\n");
    response.append("<params>\n");
    response.append("  <param><value><struct>\n");
    response.append("    <member>\n");
    response.append("      <name>samp.status</name>\n");
    response.append("      <value>samp.ok</value>\n");
    response.append("    </member>\n");
    response.append("  </struct></value></param>\n");
    response.append("</params>\n");
    response.append("</methodResponse>\n");
    
    // Post string
    post_string(response, sock);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Post SAMP void massage to client
 *
 * @param[in] sock Socket.
 *
 * Posts a void SAMP message to a client. Void messages can typically be
 * discared by the client.
 ***************************************************************************/
void GVOHub::post_samp_void(const socklen_t& sock) const
{
    // Declare response
    std::string response = "";

    // Compose response
    response.append("<?xml version='1.0' encoding='UTF-8'?>\n");
    response.append("<methodResponse>\n");
    response.append("  <params>\n");
    response.append("    <param>\n");
    response.append("      <value/>\n");
    response.append("    </param>\n");
    response.append("  </params>\n");
    response.append("</methodResponse>\n");
    
    // Post string
    post_string(response, sock);

    // Return
    return;
}


/***********************************************************************//**
 * @brief Send notification to client
 *
 * @param[in] url URL.
 * @param[in] notification Notification.
 *
 * Send a @p notification to a @p url.
 ***************************************************************************/
void GVOHub::notify(const std::string& url,
                    const std::string& notification) const
{
    // Initalise socket
    int socket = -1;

    // Extract host, port and path from URL
    std::string host = "127.0.0.1";
    std::string port = "2525";
    std::string path = "xmlrpc";
    if (url.compare(0, 7, "http://") == 0) {
        size_t length;
        size_t start = 7;
        size_t stop  = url.find(":", start);
        size_t end   = std::string::npos;
        if (stop != std::string::npos) {
            length = stop - start;
        }
        else {
            length = std::string::npos;
        }
        host = url.substr(start, length);
        if (stop != std::string::npos) {
            stop++;
            end  = url.find("/", stop);
            if (end != std::string::npos) {
                length = end - stop;
            }
            else {
                length = std::string::npos;
            }
            port = url.substr(stop, length);
        }
        if (end != std::string::npos) {
            end++;
            length = url.length() - end;
            path   = url.substr(end, length);
        }
    }

    // Set hints
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Get server information
    struct addrinfo* servinfo;
    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo) == 0) {

        // Loop through all the results and connect to the first we can
        for (struct addrinfo* ptr = servinfo; ptr != NULL; ptr = ptr->ai_next) {

            // Create socket
            socket = ::socket(ptr->ai_family,
                              ptr->ai_socktype,
                              ptr->ai_protocol);

            // Connect to socket if socket is valid
            if (socket != -1) {
                if (::connect(socket,
                              ptr->ai_addr,
                              ptr->ai_addrlen) == -1) {
                    close(socket);
                    socket = -1;
                }
                else {
                    // Socket connection successful, break now
                    break;
                }
            }

        } // endfor: looped through all results

    } // endif: server information was valid

    // Continue only if socket is valid
    if (socket != -1) {

        // Determine notification length
        int length = notification.length();

        // Set prefix
        std::string prefix = "POST /"+path+" HTTP/1.0\n"
                             "Connection: close\n"
                             "User-Agent: GammaLib\n"
                             "Content-Type: text/xml\n"
                             "Content-Length: "+gammalib::str(length)+"\n\n";

        // Build post string
        std::string post = prefix + notification;

        // Dump message to post
        #if defined(G_SHOW_MESSAGE)
        std::cout << std::endl;
        std::cout << "GVOHub sends following notification:" << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << post << std::endl;
        #endif

        // Send content to socket
        bool done = false;
        do {
            int length      = post.length() + 1; // +1 for terminating 0
            int sent_length = send(socket, post.c_str(), length, 0);
            if (sent_length < length) {
                post = post.substr(sent_length, std::string::npos);
            }
            else {
                done = true;
            }
        } while (!done);

        /*
        // Declare response
        std::string response = "";

        // Read buffer until it is empty
        char buffer[1001];
        int timeout = 1000; // Initial timeout is 1 sec
        int n       = 0;
        do {
            n = gammalib::recv(socket, buffer, 1000, 0, timeout);
            if (n > 0) {
                buffer[n+1] = '\0';
                response.append(std::string(buffer));
            }
            timeout = 10; // The timeout now is 0.01 sec 
        } while (n > 0);
        */

        // Close socket
        close(socket);

    }

    // Return
    return;
}


/***********************************************************************//**
 * @brief Generates random string of characters
 *
 * @param[in] length Length of random string
 ***************************************************************************/
std::string GVOHub::random_string(const size_t& length) const
{
    srand(time(0));
    std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int pos;
    while(str.size() != length) {
        pos = ((rand() % (str.size() - 1)));
        str.erase (pos, 1);
    }
   
    // Return string
    return str;
}

