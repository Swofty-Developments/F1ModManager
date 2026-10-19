// struct: EntrantRecord
// subsystem: session
// size: 0x88 L2 decomp:re/decomp/14245fb70.c candidate and session vectors step 0x88
// notes: in the Weekend/session participant vector the element carries a polymorphic header (vtable &UNK_145612e20 at +0, sub-object vtable &UNK_145612ed0 at +0x70, set by Sess_ParticipantVec_PushBack); those headers are not read from the source record on copy
struct EntrantRecord {
    /* 0x04b */ uint8_t flags;          // L1 doc:research/grid24-cap.md#3 bit 0x10 excludes the record from the AI count
    /* 0x050 */ uint8_t id_lo;          // L2 decomp:re/decomp/140bd1ac0.c;kb:subsystems/session.md#tyre low byte of the participant net id; matched by Sess_TyreAllocation_SendNetMessage scanning the session vector
    /* 0x052 */ uint16_t id_hi;         // L2 decomp:re/decomp/140bd1ac0.c;kb:subsystems/session.md#tyre high word of the participant net id (entry id = id_hi<<16 | id_lo; 0x3fff... = sentinel)
    /* 0x05c */ uint32_t vehicle_ref;   // L1 doc:research/grid24-cap.md#2 passed as &e+0x5c to Sess_Find_Vehicle; width unproven
};
