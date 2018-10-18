/*
 * gridappsd.hpp
 *
 *  Created on: Oct 15, 2018
 *      Author: afisher
 */
#include <functional>
#include <string>
#include <vector>
#include "json/json.h"

#ifndef GRIDAPPSD_HPP_
#define GRIDAPPSD_HPP_

namespace gridappsd {
	class DifferenceBuilder {
	private:
		std::string simulation_id;
		std::vector<Json::Value> forward;
		std::vector<Json::Value> reverse;
	public:
		DifferenceBuilder(std::string simulation_id) {
			this->simulation_id = simulation_id;
			this->clear();
		}
		void add_difference(std::string object_id, std::string attribute,
							int forward_value, int reverse_value);
		void clear(void);
		Json::Value get_message(void);
	};


	class CallbackWrapperListener {
	private:
		std::function<void(int, int)> callback;
	public:
		CallbackWrapperListener(std::function<void(std::string, std::string)> callback) {
			this->callback = callback;
		}
		void on_message(std::string header, std::string message);
	};


	class GOSS {
	private:
		std::string user;
		std::string pass;
		std::string topic;
		int id;
		//TODO: create conn equivalent in cpp
		void *conn;
		void make_connection();
	public:
		GOSS(std::string username, std::string password,
			 std::string host_address, std::string host_port, int id,
			 bool attempt_connection)
		{
			this->user = username;
			this->pass = password;
			this->host_address = host_address;
			this->host_port = host_port;
			this->conn = NULL;
			this->id = id;
			if(attempt_connection) {
				this->make_connection();
			}
		}
		std::string host_address;
		std::string host_port;
		Json::Value response;
		bool connected();
		bool connect();
		void send(std::string topic, std::string message);
		Json::Value get_response(std::string topic, std::string message, int timeout);
		void subscribe(std::string topic, std::function<void(std::string, std::string)> callback, int id);
	};


	class GridAPPSD : GOSS {
	private:
		std::string simulation_status_topic;
		std::string simulation_id;
		std::string base_status_topic;
		Json::Value build_query_payload(std::string request_type,
										std::string response_format,
										const char *kwargs, ...);
	public:
		GridAPPSD(int simulation_id, std::string source,
				  std::string base_simulation_status_topic,
				  const char *kwargs, ...)
		{

		}
		Json::Value query_object_types(std::string model_id);
		Json::Value query_model_names(std::string model_id);
		Json::Value query_model_info(void);
		Json::Value query_object(std::string object_id, std::string model_id);
		Json::Value query_data(std::string query, std::string database_type,
							   int timeout);
		Json::Value get_platform_status(bool applications, bool services,
										bool app_instances,
										bool service_instances);
		void send_simulation_status(std::string status, std::string message,
									std::string log_level);
		void send_status(std::string status, std::string topic,
						 std::string log_level);
		std::string build_message_json(std::string status, std::string message,
									   std::string log_level);
	};
}

#endif /* GRIDAPPSD_HPP_ */
