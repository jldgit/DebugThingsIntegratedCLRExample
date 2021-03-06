#pragma once

/// <summary>
///     Interface to the CLR from the unmanaged application
/// </summary>
class ATL_NO_VTABLE CClrHost :  public CComObjectRootEx<CComSingleThreadModel>,
                                public IHostControl,
                                public IUnmanagedHost
{
protected:
    BEGIN_COM_MAP(CClrHost)
        COM_INTERFACE_ENTRY(IHostControl)
        COM_INTERFACE_ENTRY(IUnmanagedHost)
    END_COM_MAP()

private:
    typedef map<DWORD, IManagedHost *>  AppDomainManagerMap;

    bool                    m_started;
    ICLRRuntimeHost         *m_pClr;
    ICLRControl             *m_pClrControl;
    AppDomainManagerMap     m_appDomainManagers;

private:
    static const wchar_t    *AppDomainManagerAssembly;
    static const wchar_t    *AppDomainManagerType;

public:
    CClrHost();
    ~CClrHost();

protected:
    HRESULT FinalConstruct();

public:
    static HRESULT BindToRuntime(__deref_in IUnmanagedHost **pHost);

    // IHostControl
public:
    STDMETHODIMP GetHostManager(const IID &riid, __deref_opt_out_opt void **ppvObject);
    STDMETHODIMP SetAppDomainManager(DWORD dwAppDomainId, __in IUnknown *pUnkAppDomainManager);

    // IUnmanagedHost
public:
    STDMETHODIMP raw_Start();
    STDMETHODIMP raw_Stop();
    STDMETHODIMP get_DefaultManagedHost(__out IManagedHost **ppHost);
    STDMETHODIMP raw_GetManagedHost(long appDomain, __out IManagedHost **ppHost);
};

// we don't have an AppDomainManager for the specified domain
#define E_NOMANAGEDHOST     MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x201);