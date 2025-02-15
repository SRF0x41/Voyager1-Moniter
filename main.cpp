#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

void scanBLEDevices() {
    int dev_id = hci_get_route(nullptr);  // Get Bluetooth adapter ID
    int sock = hci_open_dev(dev_id);  // Open Bluetooth socket

    if (dev_id < 0 || sock < 0) {
        std::cerr << "Error opening Bluetooth socket." << std::endl;
        return;
    }

    // Set BLE scan parameters
    uint8_t scan_type = 0x01;  // Active scanning (0x00 = Passive)
    uint16_t interval = htobs(0x0010);  // Scan interval
    uint16_t window = htobs(0x0010);  // Scan window
    uint8_t own_type = 0x00;  // Public Device Address
    uint8_t filter_policy = 0x00;  // Accept all advertising packets

    if (hci_le_set_scan_parameters(sock, scan_type, interval, window, own_type, filter_policy, 1000) < 0) {
        std::cerr << "Failed to set scan parameters." << std::endl;
        close(sock);
        return;
    }

    // Start scanning
    if (hci_le_set_scan_enable(sock, 0x01, 1, 1000) < 0) {
        std::cerr << "Failed to start scanning." << std::endl;
        close(sock);
        return;
    }

    std::cout << "Scanning for BLE (GATT) devices..." << std::endl;

    unsigned char buf[HCI_MAX_EVENT_SIZE];
    evt_le_meta_event *meta;
    le_advertising_info *info;
    int len;

    while (true) {
        len = read(sock, buf, sizeof(buf));
        if (len < 0) continue;

        if (buf[0] == HCI_EVENT_PKT && buf[1] == EVT_LE_META_EVENT) {
            meta = (evt_le_meta_event *)(buf + (1 + HCI_EVENT_HDR_SIZE));

            if (meta->subevent == EVT_LE_ADVERTISING_REPORT) {
                info = (le_advertising_info *)(meta->data + 1);
                char addr[19] = {0};
                ba2str(&info->bdaddr, addr);

                std::cout << "Found BLE Device: " << addr << std::endl;
            }
        }
    }

    hci_le_set_scan_enable(sock, 0x00, 1, 1000);  // Stop scanning
    close(sock);
}

int main() {
    scanBLEDevices();
    return 0;
}
