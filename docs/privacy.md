# Privacy

sACNView does not collect or store personally identifiable information (PII). This privacy policy explains what data the app transmits, how it is used, and the limited security implications of that transmission.

Effective date: February 22, 2026

Summary
- **No personal data collection:** sACNView does not collect, store, or transmit users' names, email addresses, device owner information, location data, or any other information that could be used to identify an individual.
- **Network transmission:** The app transmits lighting-control data (sACN/DMX-like lighting data) over the network in unencrypted form. This data does not contain PII but may include lighting device addresses, universe numbers, channel values, and session-related control messages.

Information We Do Not Collect
- **Identifiers & contact info:** We do not collect device identifiers, account credentials, contact lists, or other personally identifying information.
- **Usage analytics:** The app does not send usage analytics, telemetry, or crash reports to third-party services unless you explicitly enable such functionality via a future opt-in setting.

Information We Transmit
- **Lighting control data:** sACNView sends and receives lighting-control packets necessary to operate lighting fixtures and visualizers. Examples include universe/channel values and session control messages. These are transmitted in the clear (unencrypted) using standard protocols compatible with lighting equipment.

How We Use Transmitted Data
- **Operational only:** Transmitted lighting data is used solely to control and visualize lighting equipment during active sessions. The app does not retain this data beyond the lifetime of the transmission/session.

Sharing and Third Parties
- **No sharing of PII:** Because the app does not gather PII, there is no PII sharing with third parties.
- **Network endpoints:** Lighting data is sent to devices or services you choose to connect to (for example, networked lighting fixtures, bridges, or other control software). Those endpoints may log or process data according to their own policies; sACNView does not control third-party handling of transmitted data.

Security and Transmission Risks
- **Unencrypted transmission:** sACNView transmits lighting-control data in unencrypted formats. While this data is not personally identifiable, transmitting any data in cleartext carries the risk of interception by others on the same network. If you operate on untrusted networks, assume that lighting data may be visible to other devices on that network.
- **User responsibility:** Users are responsible for choosing secure networks and for configuring any intermediate devices (routers, switches, firewalls) appropriately. If you require confidentiality for lighting control data, use network infrastructure that provides encryption (VPNs, secure VLANs, or other transport security) between participating endpoints.

Data Retention
- **No retention of PII:** The app does not store personal data.
- **Session data:** Any lighting session state is transient and only exists while a session is active in the app. No persistent server-side storage is used by the app itself.

Children's Privacy
- sACNView is not designed for use by children under 13. Because the app does not collect PII, there is no deliberate collection of children's data.

Changes to This Policy
- We may update this policy to reflect changes in the app or legal requirements. The effective date at the top will be updated when we make material changes.

Contact
- For questions about this policy or about how sACNView handles data, please refer to the project README or the app listing for developer contact information.

Acknowledgment
- By using sACNView you acknowledge that lighting-control data will be transmitted in cleartext and accept the associated network risks. If you have concerns about network security, take appropriate measures (secure networks, VPNs, or encrypted tunnels) before operating the app on public or untrusted networks.
