import logging
import os
from logging.handlers import RotatingFileHandler, TimedRotatingFileHandler

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


def _resolve_log_dir() -> str:
    if not os.path.exists(LOG_DIR):
        try:
            os.makedirs(LOG_DIR)
            return LOG_DIR
        except OSError:
            os.makedirs("./log", exist_ok=True)
            return "./log"
    return LOG_DIR


def setup_logging() -> logging.Logger:
    log_dir = _resolve_log_dir()
    level = _resolve_log_level()
    formatter = logging.Formatter(LOG_FORMAT)

    # --- Logger principal: app.log (rotação diária, 30 dias) ---
    main_logger = logging.getLogger("biodoc_intelbras")
    main_logger.setLevel(level)

    if not main_logger.handlers:
        file_handler = TimedRotatingFileHandler(
            os.path.join(log_dir, "app.log"),
            when="midnight",
            interval=1,
            backupCount=30,
            encoding="utf-8",
        )
        file_handler.suffix = "%Y-%m-%d"
        file_handler.setFormatter(formatter)
        main_logger.addHandler(file_handler)

        console_handler = logging.StreamHandler()
        console_handler.setFormatter(formatter)
        main_logger.addHandler(console_handler)

    # --- Logger keep-alive: keep_alive.log (rotação por tamanho, max 1 MB x 3) ---
    ka_logger = logging.getLogger("biodoc_intelbras.keep_alive")
    ka_logger.setLevel(logging.DEBUG)
    ka_logger.propagate = False  # não duplica no app.log

    if not ka_logger.handlers:
        ka_handler = RotatingFileHandler(
            os.path.join(log_dir, "keep_alive.log"),
            maxBytes=1 * 1024 * 1024,
            backupCount=3,
            encoding="utf-8",
        )
        ka_handler.setFormatter(formatter)
        ka_logger.addHandler(ka_handler)

        ka_console = logging.StreamHandler()
        ka_console.setFormatter(formatter)
        ka_logger.addHandler(ka_console)

    return main_logger


logger = setup_logging()
keep_alive_logger = logging.getLogger("biodoc_intelbras.keep_alive")
