#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>

void scanBluetoothDevices() {
    int dev_id = hci_get_route(nullptr);
    int sock = hci_open_dev(dev_id);

    if (dev_id < 0 || sock < 0) {
        std::cerr << "Error opening Bluetooth socket." << std::endl;
        return;
    }

    inquiry_info *ii = new inquiry_info[8];
    int num_rsp = hci_inquiry(dev_id, 8, 8, nullptr, &ii, IREQ_CACHE_FLUSH);

    if (num_rsp < 0) {
        std::cerr << "Inquiry failed." << std::endl;
        return;
    }

    char addr[19] = {0};
    char name[248] = {0};

    for (int i = 0; i < num_rsp; i++) {
        ba2str(&(ii + i)->bdaddr, addr);
        if (hci_read_remote_name(sock, &(ii + i)->bdaddr, sizeof(name), name, 0) < 0) {
            strcpy(name, "[unknown]");
        }
        std::cout << "Found Device: " << addr << " - " << name << std::endl;
    }

    delete[] ii;
    close(sock);
}

int main() {
    std::cout << "Scanning for Bluetooth devices..." << std::endl;
    scanBluetoothDevices();
    return 0;
}
