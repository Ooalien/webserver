#include "request.hpp"

void	end_of_headers(request *req, client *cl)
{
	if (req->data.find("Transfer-Encoding:") != req->data.end())
	{
		if (req->data.find("Transfer-Encoding:")->second == "chunked")
			cl->chunked = 1;
	}
	if (req->data.find("Content-Length:") != req->data.end() && cl->chunked == 0)
	{
		try {
			cl->read_len = stoi(req->data.find("Content-Length:")->second);
		} catch (std::exception &x){
			std::cout << x.what() << std::endl;
		}
	}
	if (cl->read_len == 0 && cl->chunked == 0)
		cl->read_status = 1;
}


void	request::handle_body(client *cl)
{
	std::ostringstream  ss1;
	std::ifstream myfile;

	ss1 << "/tmp/request_" << cl->client_fd;
	myfile.open(ss1.str());
	if (myfile.fail())
		exit(0);
}

request::request(client *cl)
{
	std::ifstream myfile;
	std::ofstream myfile1;
	std::ostringstream  ss1;
	std::ostringstream  ss2;
	std::string tmp;
	int i = 0;
	ss1 << "/tmp/request_" << cl->client_fd;
	ss2 << "/tmp/body_" << cl->client_fd;
	myfile.open(ss1.str());
	if (cl->flag == 0)
		myfile1.open(ss2.str());
	if (myfile.fail())
		exit(0);
	while (getline(myfile, tmp))
	{
		std::istringstream iss(tmp);
		if (tmp == "\r") // if it is the end of body read status is 1 else if it's the end of request 2
		{
			if (cl->flag == 0)
			{
				cl->flag = 1;
				end_of_headers(this ,cl);
			}
			else
			{
				cl->flag = 2;
				cl->read_status = 1;
			}
			std::cout << ">>>.<<<" << cl->flag << "   status  ->>"<< cl->read_status << std::endl;
		}
		else if (i == 0 && cl->flag == 0)
		{
			iss >> method;
			iss >> absoluteURI;
			iss >> http_version;
		}
		else if (tmp.find(":") != std::string::npos && cl->flag == 0)
		{
			std::string tmp2;
			std::string tmp3;
			iss >> tmp2;
			iss >> tmp3;
			std::pair<std::string, std::string> pr;
			pr.first = tmp2;
			pr.second = tmp3;
			data.insert(pr);
			// std::cout << ">>> map insertion. <<<" << std::endl;
			// if (tmp2 == "Content-length")
			// {
			// 	try {
			// 		cl->read_len = stoi(tmp3);
			// 	} catch (std::exception &x){
			// 		std::cout << x.what() << std::endl;
			// 	}
			// }
			// else if (tmp2 == "Transfer-Encoding")
			// {
			// 	if (tmp3 == "chunked")
			// 		;/////////////////////// need variable;
			// }

		}
		else{
			std::cout << tmp;
			std::cout << "\n";
			cl->readed += tmp.size() + 1;
		}
		i++;
	}
// 	--(cl->readed);
// 	std::cout << "[" << cl->readed << "] <----->  this is cl->readed" << std::endl;
}
request::request(){}