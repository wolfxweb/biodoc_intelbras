import logging
import os
from logging.handlers import TimedRotatingFileHandler

LOG_DIR = "/log"
LOG_FORMAT = "%(asctime)s - %(name)s - %(levelname)s - %(message)s"

_LEVEL_MAP = {
    "DEBUG": logging.DEBUG,
    "INFO": logging.INFO,
    "WARNING": logging.WARNING,
    "ERROR": logging.ERROR,
    "CRITICAL": logging.CRITICAL,
}


def _resolve_log_level() -> int:
    raw = os.getenv("LOG_LEVEL", "INFO").upper().strip()
    return _LEVEL_MAP.get(raw, logging.INFO)


def setup_logging():
    if not os.path.exists(LOG_DIR):
        try:
            os.makedirs(LOG_DIR)
        except OSError:
            os.makedirs("./log", exist_ok=True)
            log_path = "./log/app.log"
        else:
            log_path = os.path.join(LOG_DIR, "app.log")
    else:
        log_path = os.path.join(LOG_DIR, "app.log")

    level = _resolve_log_level()
    logger = logging.getLogger("biodoc_intelbras")
    logger.setLevel(level)

    if not logger.handlers:
        handler = TimedRotatingFileHandler(
            log_path,
            when="midnight",
            interval=1,
            backupCount=30,
            encoding="utf-8",
        )
        handler.suffix = "%Y-%m-%d"
        formatter = logging.Formatter(LOG_FORMAT)
        handler.setFormatter(formatter)
        logger.addHandler(handler)

        console_handler = logging.StreamHandler()
        console_handler.setFormatter(formatter)
        logger.addHandler(console_handler)

    return logger


logger = setup_logging()
