
import unreal

@unreal.uclass()
class DungeonScalerUtility(unreal.GlobalEditorUtilityBase):
    pass

# =====================================
# 파라미터 (여기만 수정하세요)
# =====================================
scale_multiplier = 3.0  # 스케일 배수

# =====================================
# 현재 레벨에 배치된 모든 StaticMeshActor 스케일 조정
# =====================================

all_actors = unreal.EditorLevelLibrary.get_all_level_actors()
count = 0

for actor in all_actors:
    if isinstance(actor, unreal.StaticMeshActor):
        actor_root = actor.get_editor_property("root_component")
        old_scale = actor_root.get_editor_property("relative_scale3d")
        new_scale = unreal.Vector(
            old_scale.x * scale_multiplier,
            old_scale.y * scale_multiplier,
            old_scale.z * scale_multiplier
        )
        actor_root.set_editor_property("relative_scale3d", new_scale)
        count += 1
        print(f"스케일 변경됨: {actor.get_name()} → {new_scale}")

unreal.EditorLevelLibrary.save_current_level()
print(f"===== 던전 스케일러 완료 ({count}개 액터 수정됨) =====")
