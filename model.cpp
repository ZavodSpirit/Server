#include "model.h"

static std::mutex mtxS;

namespace model {
	Model::Model() noexcept {
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 2);
		status = true;
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			status = false;
			return;
		}

		sListen = socket(AF_INET, SOCK_STREAM, NULL);
		if (sListen == INVALID_SOCKET) {
			status = false;
			return;
		}

		in_addr ip_to_num;
		if (inet_pton(AF_INET, "127.0.0.1", &ip_to_num) <= 0) {
			status = false;
			return;
		}
		ZeroMemory(&servInfo, sizeof(servInfo));
		servInfo.sin_family = AF_INET;
		servInfo.sin_addr = ip_to_num;
		servInfo.sin_port = htons(1111);

		if (bind(sListen, (sockaddr*)&servInfo, sizeof(servInfo)) != 0) {
			status = false;
			return;
		}

		if (listen(sListen, SOMAXCONN) != 0) {
			status = false;
		}
	}

	Model::~Model() {
		for (auto it = sConnections.begin(), end = sConnections.end(); it != end; ++it)
			closesocket(*it);
		closesocket(sListen);
		WSACleanup();
	}
}

static std::wstring CreateUser(std::wstring domain, sockaddr_in clientInfo) noexcept {
	char buffer[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &clientInfo.sin_addr.s_addr, buffer, sizeof(buffer)) == NULL)
		return L"Error";
	return domain + std::wstring(buffer, buffer + INET_ADDRSTRLEN);
}

namespace model {
	void Model::Listen(HWND hWnd) noexcept {
		while (true) {
			sockaddr_in clientInfo = {};
			ZeroMemory(&clientInfo, sizeof(clientInfo));
			int clientInfo_size = sizeof(clientInfo);
			SOCKET ClientConn = accept(sListen, (sockaddr*)&clientInfo, &clientInfo_size);
			if (ClientConn == INVALID_SOCKET)
				continue;
			char hostName[NI_MAXHOST];
			//if (getnameinfo((sockaddr*)&clientInfo, clientInfo_size, hostName, sizeof(hostName), NULL, 0, NI_NOFQDN) == 0)
			//	continue;
			std::wstring domain(L"Domain");
			mtxS.lock();
			if (WSAAsyncSelect(ClientConn, hWnd, WM_SOCKET + sConnections.size(), FD_CLOSE) != 0) {
				mtxS.unlock();
				continue;
			}
			sConnections.push_back(ClientConn);
			connInfo.push_back(clientInfo);
			users.push_back(CreateUser(domain, clientInfo));
			time_t now = time(0);
			tm nowData = {};
			localtime_s(&nowData, &now);
			connTime.push_back(nowData);
			mtxS.unlock();
		}
	}

	void Model::Disconnect(size_t index) noexcept {
		if (index >= sConnections.size())
			return;
		mtxS.lock();
		closesocket(sConnections.at(index));
		sConnections.erase(sConnections.begin() + index);
		connInfo.erase(connInfo.begin() + index);
		users.erase(users.begin() + index);
		connTime.erase(connTime.begin() + index);
		mtxS.unlock();
	}
}

namespace model {
	tm Model::getActiveTime(int index) const noexcept {
		if ((index >= sConnections.size()) or (index < 0))
			return {};
		char buffer[1024];
		buffer[0] = 'T';
		auto packet_size = send(sConnections.at(index), buffer, 1, 0);
		if (packet_size == SOCKET_ERROR)
			return {};
		packet_size = recv(sConnections.at(index), buffer, 1024, 0);
		if (packet_size == SOCKET_ERROR)
			return {};
		auto secs = atoll(buffer);
		tm locTime = {};
		localtime_s(&locTime, &secs);
		return locTime;
	}
}

namespace model {
	void Model::getPrintScreen(int index) const noexcept {
		if ((index >= sConnections.size()) or (index < 0))
			return;
		char* buffer = new char[2 * 1024 * 1024];
		buffer[0] = 'P';
		auto packet_size = send(sConnections.at(index), buffer, 1, 0);
		if (packet_size == SOCKET_ERROR)
			return;
		packet_size = recv(sConnections.at(index), buffer, 2*1024*1024, 0);
		if (packet_size == SOCKET_ERROR)
			return;
		std::ofstream output(L"screen.png", std::ios::binary);
		output.write(buffer, packet_size);
		output.close();
		delete[] buffer;
	}
}

namespace model {
	std::vector<std::wstring> Model::getUsers() const noexcept {
		return users;
	}

	std::vector<tm> Model::getConnTime() const noexcept {
		return connTime;
	}
}