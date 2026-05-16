from src.core.database import engine, Base
from src.core.lifespan import lifespan
from src.api.routes.sources import router as sources_router
from src.api.routes.sync import router as sync_router
from fastapi import FastAPI

Base.metadata.create_all(bind=engine)

app = FastAPI(title="BIODOC-Intelbras Middleware", lifespan=lifespan)
app.include_router(sync_router)
app.include_router(sources_router)

@app.get("/health")
async def health_check():
    return {"status": "ok"}
