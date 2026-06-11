#pragma once

/// <summary>
/// デバッグ機能のON/OFFを一元管理するクラス
/// </summary>
class DebugManager
{
public:
    /// <summary>
    /// デバッグモードが有効かを返す
    /// </summary>
    static bool IsDebugEnabled()
    {
        return is_debug_enabled_;
    }

private:
    /// <summary>
    /// デバッグモードが有効か
    /// trueにするとデバッグ表示がONになる
    /// </summary>
    static const bool is_debug_enabled_ = false;
};