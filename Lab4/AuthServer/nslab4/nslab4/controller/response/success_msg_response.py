def make_success_msg_response(msg: str = None) -> dict:
    return {
        "success": msg is None,
        "msg": "" if msg is None else msg
    }
