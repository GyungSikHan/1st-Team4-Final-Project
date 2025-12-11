
import unreal

# =====================================
# 파라미터 (여기만 수정하세요)
# =====================================
folder_path = "/Game/Dungeon_Modular_V1"   # 던전 폴더 경로
scale_multiplier = 3.0          # 스케일 배수

# =====================================
# 작업 시작
# =====================================

asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
assets = asset_registry.get_assets_by_path(folder_path, recursive=True)

print("===== 던전 스케일러 시작 =====")
print("총 검색된 에셋 수:", len(assets))

for asset in assets:
    asset_path = asset.package_name
    static_mesh = unreal.EditorAssetLibrary.load_asset(asset_path)
    if isinstance(static_mesh, unreal.StaticMesh):
        changed = False

        num_source_models = static_mesh.get_editor_property("num_source_models")
        source_models = static_mesh.get_editor_property("source_models")

        for lod_index in range(num_source_models):
            source_model = source_models[lod_index]
            build_settings = source_model.build_settings
            current_scale = build_settings.build_scale3d
            new_scale = unreal.Vector(
                current_scale.x * scale_multiplier,
                current_scale.y * scale_multiplier,
                current_scale.z * scale_multiplier
            )
            if new_scale != current_scale:
                build_settings.build_scale3d = new_scale
                source_model.build_settings = build_settings
                changed = True
                print(f"스케일 변경됨: {static_mesh.get_name()} → {new_scale}")

        if changed:
            static_mesh.post_edit_change()
            unreal.EditorAssetLibrary.save_asset(asset_path)

print("===== 던전 스케일러 완료 =====")