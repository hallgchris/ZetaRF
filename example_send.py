from datetime import datetime, timedelta
from time import sleep
from zetarf433 import PyZetaRF433, Status

PACKET_LENGTH = 16
CHANNEL = 4

if __name__ == "__main__":
    zeta = PyZetaRF433()

    print("Starting Zeta TxRx...")
    if not zeta.begin(PACKET_LENGTH):
        print("Zeta begin failed")

    part_info = zeta.readPartInformation()
    print("----------")
    print(f"Chip rev: {part_info.chip_rev}")
    print(f"PBuild  : {part_info.pbuild}")
    print(f"Customer: {part_info.customer}")
    print(f"Rom ID  : {part_info.rom_id}")
    print(f"Bond    : {part_info.bond}")
    print()

    func_info = zeta.readFunctionRevisionInformation()
    print("----------")
    print(f"Rev Ext   : {func_info.rev_ext}")
    print(f"Rev Branch: {func_info.rev_branch}")
    print(f"Rev Int   : {func_info.rev_int}")
    print(f"Func      : {func_info.func}")
    print(f"SVN Flags : {func_info.svn_flags}")
    print()

    if not zeta.start_listening_on_channel(CHANNEL):
        print("Failed to begin listening")

    print("Init done.")

    last_send_time = datetime.now()
    while True:
        now = datetime.now()
        if last_send_time + timedelta(seconds=2) < now:
            print("Sending")
            zeta.send_packet(CHANNEL, b"test")
            last_send_time = now

        if zeta.check_for(Status.DataTransmitted):
            print("Data transmitted")
